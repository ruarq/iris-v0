//
// Created by ruarq on 19/07/2024.
//

#include "CCompiler.hpp"

namespace iris {
	auto CCompiler::to_string(ast::BinaryExpr::Op const op) -> std::string_view {
		using enum ast::BinaryExpr::Op;
		switch (op) {
			case Add: return "+";
			case Sub: return "-";
			case Mul: return "*";
			case Div: return "/";
			case Mod: return "%";
			case Less: return "<";
			case Greater: return ">";
			case LessEq: return "<=";
			case GreaterEq: return ">=";
			case Eq: return "==";
			case NotEq: return "!=";
		}

		IRIS_NOT_IMPLEMENTED(iris::CCompiler::to_string(ast::BinaryExpr::Op const));
	}

	CCompiler::CCompiler(File const &file, Diagnostics &diagnostics)
		: _file{ file }
		, _diagnostics{ diagnostics } {
	}

	auto CCompiler::result() -> std::string {
		return std::move(_code);
	}

	auto CCompiler::visit(ast::TranslationUnit &translation_unit) -> void {
		create_preamble();
		println();
		for (auto const &decl : translation_unit.decls()) {
			if (auto const raw_ptr = decl.get();
				auto const fn_decl = dynamic_cast<ast::FnDecl *>(raw_ptr)) {
				write_signature(*fn_decl);
				println(";");
			}
		}

		println();

		for (auto const &decl : translation_unit.decls()) {
			write_indent();
			decl->accept(*this);
			println();
		}
	}

	auto CCompiler::visit(ast::Block &block) -> void {
		println("{{");
		indent();
		for (auto const &stmt : block.stmts()) {
			write_indent();
			stmt->accept(*this);
		}
		dedent();
		write_indent();
		println("}}");
	}

	auto CCompiler::visit(ast::FnDecl &fn_decl) -> void {
		write_signature(fn_decl);
		print(" ");
		visit(fn_decl.block());
	}

	auto CCompiler::visit(ast::ReturnStmt &return_stmt) -> void {
		print("return ");
		return_stmt.expr()->accept(*this);
		println(";");
	}

	auto CCompiler::visit(ast::ExprStmt &expr_stmt) -> void {
		expr_stmt.expr()->accept(*this);
		println(";");
	}

	auto CCompiler::visit(ast::MutStmt &mut_stmt) -> void {
		print("{} = ", generate_decl(mut_stmt.identifier(), *mut_stmt.type_name().type()));
		mut_stmt.expr()->accept(*this);
		println(";");
	}

	auto CCompiler::visit(ast::LetStmt &let_stmt) -> void {
		print("{} = ", generate_decl(let_stmt.identifier(), *let_stmt.type_name().type()));
		let_stmt.expr()->accept(*this);
		println(";");
	}

	auto CCompiler::visit(ast::IfStmt &if_stmt) -> void {
		print("if (");
		if_stmt.condition()->accept(*this);
		print(") ");
		visit(if_stmt.then());
		for (auto &elif : if_stmt.elifs()) {
			write_indent();
			print("else ");
			visit(*elif);
		}
		if (if_stmt.has_orelse()) {
			write_indent();
			print("else ");
			visit(if_stmt.orelse());
		}
	}

	auto CCompiler::visit(ast::WhileStmt &while_stmt) -> void {
		print("while (");
		while_stmt.condition()->accept(*this);
		print(") ");
		while_stmt.block().accept(*this);
	}

	auto CCompiler::visit(ast::AssignExpr &assign_expr) -> void {
		print("(");
		assign_expr.target()->accept(*this);
		print(") = (");
		assign_expr.value()->accept(*this);
		print(")");
	}

	auto CCompiler::visit(ast::NameExpr &name_expr) -> void {
		print("{}", name_expr.identifier().span().string(_file));
	}

	auto CCompiler::visit(ast::BinaryExpr &binary_expr) -> void {
		print("(");
		binary_expr.left()->accept(*this);
		print(") {} (", to_string(binary_expr.op()));
		binary_expr.right()->accept(*this);
		print(")");
	}

	auto CCompiler::visit(ast::LiteralExpr &literal_expr) -> void {
		using enum ast::Value::Kind;
		switch (literal_expr.value().kind()) {
			case Int: print("{}", literal_expr.value().get<i64>()); break;
			case Bool: print("{}", literal_expr.value().get<bool>() ? "1" : "0"); break;
		}
	}

	auto CCompiler::visit(ast::CallExpr &call_expr) -> void {
		print("(");
		call_expr.fn()->accept(*this);
		print(")");
		print("(");
		auto itr = call_expr.args().begin();
		if (!call_expr.args().empty()) {
			print("(");
			itr->expr()->accept(*this);
			print(")");
			if (itr->is_named()) {
				_diagnostics.emit(itr->span(), "CCompiler.NamedArgsNotSupported");
			}
			++itr;
		}
		for (; itr != call_expr.args().end(); ++itr) {
			print(", ");
			print("(");
			itr->expr()->accept(*this);
			print(")");
			if (itr->is_named()) {
				_diagnostics.emit(itr->span(), "CCompiler.NamedArgsNotSupported");
			}
		}
		print(")");
	}

	auto CCompiler::visit(ast::GroupExpr &group_expr) -> void {
		print("(");
		group_expr.expr()->accept(*this);
		print(")");
	}

	auto CCompiler::visit(ast::PipeExpr &pipe_expr) -> void {
		pipe_expr.target()->accept(*this);
		print("(");
		pipe_expr.value()->accept(*this);
		print(")");
	}

	auto CCompiler::create_preamble() -> void {
		println("#define true 1");
		println("#define false 0");

		println("#include <stdio.h>");
		println(R"(#define puti(x) printf("%d\n", (x)))");

		println("#include <stdint.h>");
		println("#include <stddef.h>");
	}

	auto CCompiler::write_signature(ast::FnDecl &fn_decl) -> void {
		print("{} {}(",
			type_to_ctype(*fn_decl.type_name().type()),
			fn_decl.identifier().span().string(_file));

		auto format_param = [this](auto &param) {
			auto itr = param.identifiers().begin();
			print("{} {}", type_to_ctype(*param.type_name().type()), itr->span().string(_file));
			++itr;

			for (; itr != param.identifiers().end(); ++itr) {
				print(", {} {}",
					type_to_ctype(*param.type_name().type()),
					itr->span().string(_file));
			}
		};

		auto itr = fn_decl.params().begin();
		if (!fn_decl.params().empty()) {
			format_param(*itr++);
		}

		for (; itr != fn_decl.params().end(); ++itr) {
			_code += ", ";
			format_param(*itr);
		}

		_code += ")";
	}

	auto CCompiler::indent() -> void {
		++_indent_level;
	}

	auto CCompiler::dedent() -> void {
		--_indent_level;
	}

	auto CCompiler::write_indent() -> void {
		for (usz i = 0; i < _indent_level; ++i) {
			print("\t");
		}
	}

	auto CCompiler::println() -> void {
		_code += "\n";
	}

	auto CCompiler::type_to_ctype(types::Type &type, std::string_view const identifier)
		-> std::string {
		using enum types::SimpleType::Kind;

		if (type.is_unit_type()) {
			return "void";
		}

		if (type.is_simple_type(Int8)) {
			return "int8_t";
		}

		if (type.is_simple_type(Int16)) {
			return "int16_t";
		}

		if (type.is_simple_type(Int32)) {
			return "int32_t";
		}

		if (type.is_simple_type(Int64)) {
			return "int64_t";
		}

		if (type.is_simple_type(UInt8)) {
			return "uint8_t";
		}

		if (type.is_simple_type(UInt16)) {
			return "uint16_t";
		}

		if (type.is_simple_type(UInt32)) {
			return "uint32_t";
		}

		if (type.is_simple_type(UInt64)) {
			return "uint64_t";
		}

		if (type.is_simple_type(USize)) {
			return "size_t";
		}

		if (type.is_simple_type(ISize)) {
			return "ssize_t";
		}

		if (type.is_simple_type(Bool)) {
			return "uint8_t";
		}

		if (type.is_fn_type()) {
			auto &fn_type = *reinterpret_cast<types::FnType *>(&type);
			std::string strtype =
				fmt::format("{}(*{})", type_to_ctype(*fn_type.type()), identifier);

			strtype += "(";
			auto itr = fn_type.params().begin();
			if (!fn_type.params().empty()) {
				strtype += type_to_ctype(*(itr++)->second);
			}
			for (; itr != fn_type.params().end(); ++itr) {
				strtype += ", ";
				strtype += type_to_ctype(*itr->second);
			}
			strtype += ")";

			return strtype;
		}

		return "NO_iris::types::Type_TO_CTYPE_CONVERSION";
	}

	auto CCompiler::generate_decl(ast::Identifier const &identifier, types::Type &type)
		-> std::string {
		if (type.is_fn_type()) {
			return type_to_ctype(type, identifier.span().string(_file));
		}

		return fmt::format("{} {}{}",
			type_to_ctype(type),
			type.is_read_only() ? "const " : "",
			identifier.span().string(_file));
	}

}