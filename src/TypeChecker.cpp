//
// Created by ruarq on 19/07/2024.
//

#include "TypeChecker.hpp"

namespace iris {
	TypeChecker::TypeChecker(File const &file, Diagnostics &diagnostics)
		: _file{ file }
		, _diagnostics{ diagnostics } {
	}

	auto TypeChecker::visit(ast::TranslationUnit &translation_unit) -> void {
		_environment = &translation_unit.environment();

		using enum types::SimpleType::Kind;
		auto irisc_builtin_puti =
			make_fn_type(make_params(std::pair{ "num", make_simple_type(Int32) }),
				make_simple_type(Int32));
		_environment->insert("puti", std::move(irisc_builtin_puti));

		for (auto const &decl : translation_unit.decls()) {
			if (auto const raw_ptr = decl.get();
				auto const fn_decl_ptr = dynamic_cast<ast::FnDecl *>(raw_ptr)) {
				auto &fn_decl = *fn_decl_ptr;

				std::string const fn_identifier{ fn_decl.identifier().span().string(_file) };
				std::vector<types::FnType::Param> param_types;

				for (auto &param : fn_decl.params()) {
					for (auto const &identifier : param.identifiers()) {
						std::string id{ identifier.span().string(_file) };
						auto type = param.type_name().type()->clone();
						param_types.emplace_back(std::move(id), std::move(type));
					}
				}

				_environment->insert(fn_identifier,
					std::make_unique<types::FnType>(std::move(param_types),
						fn_decl.type_name().type()->clone()));
			}
		}

		for (auto const &decl : translation_unit.decls()) {
			decl->accept(*this);
		}
	}

	auto TypeChecker::visit(ast::Block &block) -> void {
		for (auto const &stmt : block.stmts()) {
			stmt->accept(*this);
		}
	}

	auto TypeChecker::visit(ast::FnDecl &fn_decl) -> void {
		std::string const fn_identifier{ fn_decl.identifier().span().string(_file) };

		fn_decl.environment() = _environment->create_child();
		auto &environment = fn_decl.environment();

		for (auto &param : fn_decl.params()) {
			for (auto const &identifier : param.identifiers()) {
				std::string const id{ identifier.span().string(_file) };
				auto type = param.type_name().type()->clone();
				type->set_read_only(false);
				environment.insert(id, std::move(type));
			}
		}

		_environment = &fn_decl.environment();
		_return = fn_decl.type_name().type()->clone();
		visit(fn_decl.block());
		_return = nullptr;
	}

	auto TypeChecker::visit(ast::ReturnStmt &return_stmt) -> void {
		return_stmt.expr()->accept(*this);
		auto const &expr_type = return_stmt.expr()->type();
		if (*_return != *expr_type) {
			_diagnostics.emit(return_stmt.expr()->span(),
				"ExpectedType",
				_return->to_string(),
				expr_type->to_string());
		}
	}

	auto TypeChecker::visit(ast::ExprStmt &expr_stmt) -> void {
		expr_stmt.expr()->accept(*this);
	}

	auto TypeChecker::visit(ast::MutStmt &mut_stmt) -> void {
		auto &type = mut_stmt.type_name().type();

		if (mut_stmt.expr()) {
			auto const &expr_type = mut_stmt.expr()->type();
			mut_stmt.expr()->accept(*this);
			if (!type) {
				type = expr_type->clone();
			} else if (*type != *expr_type) {
				throw std::runtime_error{ "Diagnostic message." };
			}
		}

		type->set_read_only(false);
		std::string const identifier{ mut_stmt.identifier().span().string(_file) };
		_environment->insert(identifier, mut_stmt.type_name().type()->clone());
	}

	auto TypeChecker::visit(ast::LetStmt &let_stmt) -> void {
		let_stmt.expr()->accept(*this);
		auto &type = let_stmt.type_name().type();
		if (!type) {
			type = let_stmt.expr()->type()->clone();
		} else if (*type != *let_stmt.expr()->type()) {
			_diagnostics.emit(let_stmt.span(),
				"TypesDontMatch",
				type->to_string(),
				let_stmt.expr()->type()->to_string());
		}

		type->set_read_only(true);
		std::string const identifier{ let_stmt.identifier().span().string(_file) };
		_environment->insert(identifier, type->clone());
	}

	auto TypeChecker::visit(ast::IfStmt &if_stmt) -> void {
		if_stmt.condition()->accept(*this);
		if (!if_stmt.condition()->type()->is_simple_type(types::SimpleType::Kind::Bool)) {
			_diagnostics.emit(if_stmt.condition()->span(),
				"ExpectedType",
				"bool",
				if_stmt.condition()->type()->to_string());
		}

		visit(if_stmt.then());

		for (auto &elif : if_stmt.elifs()) {
			visit(*elif);
		}

		if (if_stmt.has_orelse()) {
			visit(if_stmt.orelse());
		}
	}

	auto TypeChecker::visit(ast::WhileStmt &while_stmt) -> void {
		while_stmt.condition()->accept(*this);
		if (!while_stmt.condition()->type()->is_simple_type(types::SimpleType::Kind::Bool)) {
			_diagnostics.emit(while_stmt.condition()->span(),
				"ExpectedType",
				"bool",
				while_stmt.condition()->type()->to_string());
		}

		visit(while_stmt.block());
	}

	auto TypeChecker::visit(ast::AssignExpr &assign_expr) -> void {
		assign_expr.target()->accept(*this);
		assign_expr.value()->accept(*this);

		auto const &target_type = assign_expr.target()->type();
		if (auto const &value_type = assign_expr.value()->type(); *target_type != *value_type) {
			_diagnostics.emit(assign_expr.span(),
				"TypesDontMatch",
				assign_expr.target()->type()->to_string(),
				assign_expr.value()->type()->to_string());
			assign_expr.type() = std::make_unique<types::ErrorType>();
			return;
		}

		if (target_type->is_read_only()) {
			_diagnostics.emit(assign_expr.target()->span(), "CantAssignToReadOnly");
		}

		assign_expr.type() = assign_expr.target()->type()->clone();
	}

	auto TypeChecker::visit(ast::NameExpr &name_expr) -> void {
		std::string const identifier{ name_expr.identifier().span().string(_file) };
		auto result = _environment->lookup(identifier);
		if (!result) {
			_diagnostics.emit(name_expr.span(), "UnknownIdentifier", identifier);
			name_expr.type() = std::make_unique<types::ErrorType>();
			return;
		}
		auto &type = *result;
		name_expr.type() = type->clone();
	}

	auto TypeChecker::visit(ast::BinaryExpr &binary_expr) -> void {
		binary_expr.left()->accept(*this);
		binary_expr.right()->accept(*this);

		using enum ast::BinaryExpr::Op;
		switch (binary_expr.op()) {
			case Add:
			case Sub:
			case Mul:
			case Div:
			case Mod: check_arith_expr(binary_expr); break;
			case Eq:
			case NotEq:
			case LessEq:
			case GreaterEq:
			case Less:
			case Greater: check_comp_expr(binary_expr); break;
		}
	}

	auto TypeChecker::visit(ast::LiteralExpr &literal_expr) -> void {
		switch (auto const &value = literal_expr.value(); value.kind()) {
			case ast::Value::Kind::Int:
				literal_expr.type() = make_simple_type(types::SimpleType::Kind::Int64);
				break;
			case ast::Value::Kind::Bool: literal_expr.type() = types::make_bool_type(); break;
		}
	}

	auto TypeChecker::visit(ast::CallExpr &call_expr) -> void {
		for (auto &arg : call_expr.args()) {
			arg.expr()->accept(*this);
		}

		call_expr.fn()->accept(*this);
		if (!call_expr.fn()->type()->is_fn_type()) {
			_diagnostics.emit(call_expr.fn()->span(),
				"ExpectedFunction",
				call_expr.fn()->type()->to_string());
			call_expr.type() = types::make_error_type();
			return;
		}

		// Doing a reinterpret cast bc we know that call_expr.fn()->type() is of type types::FnType
		auto const fn_type = reinterpret_cast<types::FnType *>(call_expr.fn()->type().get());
		auto const &params = fn_type->params();
		auto &args = call_expr.args();

		/*
		if (args.size() > params.size()) {
			_diagnostics.emit(call_expr.span(), "MismatchedArgCount", params.size(), args.size());
			call_expr.type() = types::make_error_type();
			return;
		}

		std::vector<types::FnType::Param> unmatched_param_types;

		// call(positional_args..., [named_args: expr])
		usz num_positional_args = 0;
		for (usz i = 0; i < std::min(args.size(), params.size()); ++i) {
			auto const &[_, type] = params[i];
			auto &arg = args[i];
			if (arg.is_named()) {
				break;
			}

			if (*type != *arg.expr()->type()) {
				_diagnostics.emit(arg.span(),
					"ExpectedType",
					type->to_string(),
					arg.expr()->type()->to_string());
			}

			++num_positional_args;
		}

		if (num_positional_args < params.size())
		*/

		// if (params.size() != args.size()) {
		// 	_diagnostics.emit(call_expr.span(), "MismatchedArgCount", params.size(), args.size());
		// }

		bool all_args_named = true;
		bool all_args_unnamed = true;
		for (auto &arg : args) {
			if (arg.is_named()) {
				all_args_unnamed = false;
			} else {
				all_args_named = false;
			}
		}

		if (all_args_unnamed) {
			if (params.size() != args.size()) {
				_diagnostics.emit(call_expr.span(),
					"MismatchedArgCount",
					params.size(),
					args.size());
			}

			for (usz i = 0; i < std::min(params.size(), args.size()); ++i) {
				auto const &[_, type] = params[i];
				if (auto &arg = args[i]; *arg.expr()->type() != *type) {
					_diagnostics.emit(arg.span(),
						"ExpectedType",
						type->to_string(),
						arg.expr()->type()->to_string());
				}
			}
		} else if (all_args_named) {
			auto match_param_to_arg = [this](types::FnType::Param const &param,
										  std::vector<ast::Arg> &args) {
				for (auto &arg : args) {
					auto const &[id, type] = param;
					if (id == arg.identifier().span().string(_file) &&
						*type == *arg.expr()->type()) {
						return true;
					}
				}
				return false;
			};

			std::vector<types::FnType::Param> unmatched_params;
			for (auto const &param : params) {
				if (!match_param_to_arg(param, args)) {
					unmatched_params.emplace_back(param.first, param.second->clone());
				}
			}

			if (!unmatched_params.empty()) {
				_diagnostics.emit(call_expr.span(), "ClosuresNotSupported");
				call_expr.type() =
					make_fn_type(std::move(unmatched_params), fn_type->type()->clone());
				return;
			}
		} else {
			_diagnostics.emit(call_expr.span(), "PartiallyNamedArgs");
		}

		call_expr.type() = fn_type->type()->clone();
	}

	auto TypeChecker::visit(ast::GroupExpr &group_expr) -> void {
		group_expr.expr()->accept(*this);
		group_expr.type() = group_expr.expr()->type()->clone();
	}

	auto TypeChecker::visit(ast::PipeExpr &pipe_expr) -> void {
		pipe_expr.value()->accept(*this);
		pipe_expr.target()->accept(*this);

		if (!pipe_expr.target()->type()->is_fn_type()) {
			_diagnostics.emit(pipe_expr.target()->span(),
				"ExpectedFunction",
				pipe_expr.target()->type()->to_string());

			pipe_expr.type() = types::make_error_type();
			return;
		}

		auto &fn_type = *reinterpret_cast<types::FnType *>(pipe_expr.target()->type().get());
		auto &params = fn_type.params();
		if (params.size() != 1) {
			_diagnostics.emit(pipe_expr.span(), "MismatchedArgCount", 1, params.size());
		}

		auto &[_, type] = params.front();

		if (*type != *pipe_expr.value()->type()) {
			_diagnostics.emit(pipe_expr.target()->span(),
				"ExpectedType",
				pipe_expr.value()->type()->to_string(),
				type->to_string());
		}

		pipe_expr.type() = fn_type.type()->clone();
	}

	auto TypeChecker::check_arith_expr(ast::BinaryExpr &binary_expr) -> void {
		auto const &left_type = binary_expr.left()->type();
		auto const &right_type = binary_expr.right()->type();

		if (*left_type != *right_type) {
			auto type = promote(left_type, right_type);

			if (!type) {
				_diagnostics.emit(binary_expr.span(),
					"TypesDontMatch",
					left_type->to_string(),
					right_type->to_string());
				binary_expr.type() = std::make_unique<types::ErrorType>();
				return;
			}

			binary_expr.type() = std::move(type);
			return;
		}

		binary_expr.type() = left_type->clone();
	}

	auto TypeChecker::check_comp_expr(ast::BinaryExpr &binary_expr) -> void {
		auto const &left_type = binary_expr.left()->type();
		auto const &right_type = binary_expr.right()->type();

		if (*left_type != *right_type) {
			_diagnostics.emit(binary_expr.span(),
				"TypesDontMatch",
				left_type->to_string(),
				right_type->to_string());
		}

		binary_expr.type() = types::make_bool_type();
	}

	auto TypeChecker::promote(std::unique_ptr<types::Type> const &lhs,
		std::unique_ptr<types::Type> const &rhs) -> std::unique_ptr<types::Type> {
		if (lhs->is_integral() && rhs->is_integral()) {
			if (lhs->is_signed() != rhs->is_signed()) {
				return nullptr;
			}

			if (lhs->width() > rhs->width()) {
				return lhs->clone();
			}

			return rhs->clone();
		}

		return nullptr;
	}

}