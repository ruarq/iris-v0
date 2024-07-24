//
// Created by ruarq on 19/07/2024.
//

#ifndef CALLEXPR_HPP
#define CALLEXPR_HPP

#include <optional>

#include "Expr.hpp"
#include "Identifier.hpp"

namespace iris::ast {
	class Arg {
	public:
		Arg(Arg const &) = delete;
		Arg(Arg &&) = default;
		Arg(std::unique_ptr<Expr> expr);
		Arg(Identifier const &identifier, std::unique_ptr<Expr> expr);

	public:
		[[nodiscard]] auto is_named() const -> bool;

		[[nodiscard]] auto span() const -> Span;
		[[nodiscard]] auto identifier() const -> Identifier;
		auto expr() -> std::unique_ptr<Expr> &;

	private:
		std::optional<Identifier> _identifier;
		std::unique_ptr<Expr> _expr;
	};

	IRIS_EXPR(CallExpr) {
	public:
		CallExpr(Span const &span, std::unique_ptr<Expr> fn, std::vector<Arg> args);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto fn()->std::unique_ptr<Expr> &;
		auto args()->std::vector<Arg> &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		Span _span;
		std::unique_ptr<Expr> _fn;
		std::vector<Arg> _args;
		std::unique_ptr<types::Type> _type;
	};
}

#endif	  //CALLEXPR_HPP
