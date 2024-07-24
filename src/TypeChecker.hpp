//
// Created by ruarq on 19/07/2024.
//

#ifndef TYPECHECKER_HPP
#define TYPECHECKER_HPP

#include "Diagnostics.hpp"
#include "Environment.hpp"
#include "ast/ast.hpp"

namespace iris {
	class TypeChecker : public ast::Visitor {
	public:
		explicit TypeChecker(File const &file, Diagnostics &diagnostics);

	public:
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
		auto check_arith_expr(ast::BinaryExpr &binary_expr) -> void;
		auto check_comp_expr(ast::BinaryExpr &binary_expr) -> void;
		auto promote(std::unique_ptr<types::Type> const &lhs,
			std::unique_ptr<types::Type> const &rhs) -> std::unique_ptr<types::Type>;

	private:
		File const &_file;
		Diagnostics &_diagnostics;
		Environment *_environment = nullptr;
		std::unique_ptr<types::Type> _return;
	};
}

#endif	  //TYPECHECKER_HPP
