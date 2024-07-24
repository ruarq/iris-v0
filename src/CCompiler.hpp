//
// Created by ruarq on 19/07/2024.
//

#ifndef CCOMPILER_HPP
#define CCOMPILER_HPP

#include "Diagnostics.hpp"
#include "ast/ast.hpp"

namespace iris {
	class CCompiler : public ast::Visitor {
	private:
		[[nodiscard]] static auto to_string(ast::BinaryExpr::Op op) -> std::string_view;
		static auto type_to_ctype(types::Type &type, std::string_view identifier = "")
			-> std::string;
		auto generate_decl(ast::Identifier const &identifier, types::Type &type) -> std::string;

	public:
		explicit CCompiler(File const &file, Diagnostics &diagnostics);

	public:
		[[nodiscard]] auto result() -> std::string;

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
		auto create_preamble() -> void;
		auto write_signature(ast::FnDecl &fn_decl) -> void;

		auto indent() -> void;
		auto dedent() -> void;
		auto write_indent() -> void;

		template<typename... TFormatArgs>
		auto print(fmt::format_string<TFormatArgs...> const fmt, TFormatArgs &&...format_args)
			-> void {
			_code += fmt::format(fmt, std::forward<TFormatArgs>(format_args)...);
		}

		template<typename... TFormatArgs>
		auto println(fmt::format_string<TFormatArgs...> const fmt, TFormatArgs &&...format_args)
			-> void {
			_code += fmt::format(fmt, std::forward<TFormatArgs>(format_args)...);
			println();
		}

		auto println() -> void;

	private:
		File const &_file;
		Diagnostics &_diagnostics;
		std::string _code;

		usz _indent_level = 0;
	};
}

#endif	  //CCOMPILER_HPP
