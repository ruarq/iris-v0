//
// Created by ruarq on 19/07/2024.
//

#include "AstJsonSerializer.hpp"

#define ADD_TYPE(json, node) \
	if ((node).type()) { \
		(json)["type"] = (node).type()->to_string(); \
	}

namespace iris {
	using json = nlohmann::json;

	AstJsonSerializer::AstJsonSerializer(File const &file)
		: _file{ file } {
	}

	auto AstJsonSerializer::result() -> nlohmann::json {
		if (_stack.size() != 1) {
			throw std::runtime_error{ "Unused json values on stack." };
		}
		return pop();
	}

	auto AstJsonSerializer::visit(ast::TranslationUnit &translation_unit) -> void {
		json obj{
			{ "kind", "TranslationUnit" }
		};
		for (auto const &decl : translation_unit.decls()) {
			decl->accept(*this);
			obj["decls"].push_back(pop());
		}
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::Block &block) -> void {
		json obj{
			{ "kind", "Block" }
		};
		for (auto const &stmt : block.stmts()) {
			stmt->accept(*this);
			obj["stmts"].push_back(pop());
		}
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::FnDecl &fn_decl) -> void {
		json obj{
			{ "kind",		  "FnDecl"								  },
			{ "identifier", fn_decl.identifier().span().string(_file) },
			{ "type",		  fn_decl.type_name().type()->to_string()	  },
		};
		visit(fn_decl.block());
		obj["block"] = pop();
		for (auto &param : fn_decl.params()) {
			json obj2{
				{ "kind", "Param"								  },
				{ "type", fn_decl.type_name().type()->to_string() }
			};
			for (auto const &identifier : param.identifiers()) {
				obj2["identifiers"].push_back(identifier.span().string(_file));
			}
			obj["params"].push_back(obj2);
		}
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::ReturnStmt &return_stmt) -> void {
		json obj{
			{ "kind", "ReturnStmt" },
		};
		return_stmt.expr()->accept(*this);
		obj["expr"] = pop();
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::ExprStmt &expr_stmt) -> void {
		json obj{
			{ "kind", "ExprStmt" },
		};
		expr_stmt.expr()->accept(*this);
		obj["expr"] = pop();
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::MutStmt &mut_stmt) -> void {
		json obj{
			{ "kind",		  "MutStmt"								  },
			{ "identifier", mut_stmt.identifier().span().string(_file) },
		};
		if (mut_stmt.expr()) {
			mut_stmt.expr()->accept(*this);
			obj["expr"] = pop();
		}
		ADD_TYPE(obj, mut_stmt.type_name());
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::LetStmt &let_stmt) -> void {
		json obj{
			{ "kind",		  "LetStmt"								  },
			{ "identifier", let_stmt.identifier().span().string(_file) },
		};
		let_stmt.expr()->accept(*this);
		obj["expr"] = pop();
		ADD_TYPE(obj, let_stmt.type_name());
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::IfStmt &if_stmt) -> void {
		IRIS_NOT_IMPLEMENTED(iris::AstJsonSerializer::visit(ast::IfStmt &));
	}

	auto AstJsonSerializer::visit(ast::WhileStmt &while_stmt) -> void {
		IRIS_NOT_IMPLEMENTED(iris::AstJsonSerializer::visit(ast::WhileStmt &));
	}

	auto AstJsonSerializer::visit(ast::AssignExpr &assign_expr) -> void {
		json obj{
			{ "kind", "AssignExpr" },
		};
		assign_expr.target()->accept(*this);
		obj["target"] = pop();
		assign_expr.value()->accept(*this);
		obj["value"] = pop();
		ADD_TYPE(obj, assign_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::NameExpr &name_expr) -> void {
		json obj{
			{ "kind",		  "NameExpr"								  },
			{ "identifier", name_expr.identifier().span().string(_file) },
		};
		ADD_TYPE(obj, name_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::BinaryExpr &binary_expr) -> void {
		json obj{
			{ "kind", "BinaryExpr"				   },
			{ "op",	to_string(binary_expr.op()) },
		};
		binary_expr.left()->accept(*this);
		obj["left"] = pop();
		binary_expr.right()->accept(*this);
		obj["right"] = pop();
		ADD_TYPE(obj, binary_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::LiteralExpr &literal_expr) -> void {
		json obj{
			{ "kind",  "LiteralExpr"					},
			{ "value", literal_expr.value().to_string() },
		};
		ADD_TYPE(obj, literal_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::CallExpr &call_expr) -> void {
		json obj{
			{ "kind", "CallExpr" },
		};
		call_expr.fn()->accept(*this);
		obj["fn"] = pop();
		for (auto &arg : call_expr.args()) {
			json obj2{
				{ "kind",		  "Arg"								 },
				{ "identifier", arg.identifier().span().string(_file) },
			};
			arg.expr()->accept(*this);
			obj2["expr"] = pop();
			ADD_TYPE(obj2, *arg.expr());
			obj["args"].push_back(obj2);
		}
		ADD_TYPE(obj, call_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::GroupExpr &group_expr) -> void {
		json obj{
			{ "kind", "GroupExpr" }
		};
		group_expr.expr()->accept(*this);
		obj["expr"] = pop();
		ADD_TYPE(obj, group_expr);
		push(std::move(obj));
	}

	auto AstJsonSerializer::visit(ast::PipeExpr &pipe_expr) -> void {
		IRIS_NOT_IMPLEMENTED(iris::AstJsonSerializer::visit(ast::PipeExpr &));
	}

	auto AstJsonSerializer::push(json obj) -> void {
		_stack.push(std::move(obj));
	}

	auto AstJsonSerializer::pop() -> json {
		auto obj = _stack.top();
		_stack.pop();
		return obj;
	}

}	 // iris