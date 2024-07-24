//
// Created by ruarq on 18/07/2024.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Diagnostics.hpp"
#include "Lexer.hpp"
#include "ast/ast.hpp"
#include "types/types.hpp"

namespace iris {

	class Parser {
	public:
		static auto is_decl(Token::Kind kind) -> bool;
		static auto is_stmt(Token::Kind kind) -> bool;
		static auto is_expr(Token::Kind kind) -> bool;
		static auto is_block(Token::Kind kind) -> bool;
		static auto is_param(Token::Kind kind) -> bool;
		static auto is_type(Token::Kind kind) -> bool;
		static auto is_arg(Token::Kind kind) -> bool;

	public:
		Parser(Lexer &lexer, Diagnostics &diagnostics);

	public:
		[[nodiscard]] auto parse() -> ast::TranslationUnit;

	private:
		[[nodiscard]] auto match(Token::Kind kind) const -> bool;

		template<typename... TTokenKinds>
		[[nodiscard]] auto match_any(TTokenKinds &&...kinds) const -> bool {
			return (match(kinds) || ...);
		}

		template<typename... TTokenSequence>
		[[nodiscard]] auto match_sequence(TTokenSequence &&...sequence) const -> bool {
			usz i = 0;
			return ((_lexer.peek(i++).kind() == sequence) && ...);
		}

		auto consume(Token::Kind kind) const -> Token;

		auto parse_translation_unit() -> ast::TranslationUnit;

		auto parse_decls() -> std::vector<std::unique_ptr<ast::Decl>>;
		auto parse_decl() -> std::unique_ptr<ast::Decl>;
		auto parse_fn_decl() -> std::unique_ptr<ast::FnDecl>;
		auto parse_params() -> std::vector<ast::Param>;
		auto parse_param() -> ast::Param;
		auto parse_identifiers() -> std::vector<ast::Identifier>;
		auto parse_identifier() -> ast::Identifier;
		auto parse_optional_type() -> ast::TypeName;

		auto parse_block() -> ast::Block;
		auto parse_stmts() -> std::vector<std::unique_ptr<ast::Stmt>>;
		auto parse_stmt() -> std::unique_ptr<ast::Stmt>;
		auto parse_let_stmt() -> std::unique_ptr<ast::LetStmt>;
		auto parse_mut_stmt() -> std::unique_ptr<ast::MutStmt>;
		auto parse_return_stmt() -> std::unique_ptr<ast::ReturnStmt>;
		auto parse_expr_stmt() -> std::unique_ptr<ast::ExprStmt>;
		auto parse_if_stmt() -> std::unique_ptr<ast::IfStmt>;
		auto parse_elif_stmts() -> std::vector<ast::IfStmt::ElifStmt>;
		auto parse_elif_stmt() -> ast::IfStmt::ElifStmt;
		auto parse_while_stmt() -> std::unique_ptr<ast::WhileStmt>;

		auto parse_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_assign_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_equality_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_comparison_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_add_sub_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_op() -> ast::BinaryExpr::Op;
		auto parse_mul_div_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_pipe_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_call_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_args() -> std::vector<ast::Arg>;
		auto parse_arg(bool &allow_positional) -> ast::Arg;
		auto parse_term_expr() -> std::unique_ptr<ast::Expr>;
		auto parse_group_expr() -> std::unique_ptr<ast::GroupExpr>;
		auto parse_literal_expr() -> std::unique_ptr<ast::LiteralExpr>;
		auto parse_name_expr() -> std::unique_ptr<ast::NameExpr>;

		auto parse_type_name() -> ast::TypeName;
		auto parse_type() -> std::pair<Span, std::unique_ptr<types::Type>>;

	private:
		Lexer &_lexer;
		Diagnostics &_diagnostics;
	};

}	 // iris

#endif	  //PARSER_HPP
