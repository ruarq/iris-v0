//
// Created by ruarq on 19/07/2024.
//

#ifndef ASTJSONSERIALIZER_HPP
#define ASTJSONSERIALIZER_HPP

#include <stack>

#include <nlohmann/json.hpp>

#include "ast/ast.hpp"

namespace iris {

	class AstJsonSerializer : public ast::Visitor {
	public:
		explicit AstJsonSerializer(File const &file);

	public:
		[[nodiscard]] auto result() -> nlohmann::json;

		auto visit(ast::TranslationUnit &translation_unit) -> void override;
		auto visit(ast::Block &block) -> void override;

		auto visit(ast::FnDecl &fn_decl) -> void override;

		auto visit(ast::ReturnStmt &return_stmt) -> void override;
		auto visit(ast::ExprStmt &expr_stmt) -> void override;
		auto visit(ast::MutStmt &mut_stmt) -> void override;
		auto visit(ast::LetStmt &let_stmt) -> void override;
		auto visit(ast::IfStmt &if_stmt) -> void override;
		auto visit(ast::WhileStmt &while_stmt) -> void override;

		auto visit(ast::AssignExpr &assign_expr) -> void override;
		auto visit(ast::NameExpr &name_expr) -> void override;
		auto visit(ast::BinaryExpr &binary_expr) -> void override;
		auto visit(ast::LiteralExpr &literal_expr) -> void override;
		auto visit(ast::CallExpr &call_expr) -> void override;
		auto visit(ast::GroupExpr &group_expr) -> void override;
		auto visit(ast::PipeExpr &pipe_expr) -> void override;

	private:
		auto push(nlohmann::json obj) -> void;
		auto pop() -> nlohmann::json;

	private:
		std::stack<nlohmann::json> _stack;
		File const &_file;
	};

}	 // iris

#endif	  //ASTJSONSERIALIZER_HPP
