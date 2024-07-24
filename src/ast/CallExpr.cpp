//
// Created by ruarq on 19/07/2024.
//

#include "CallExpr.hpp"

namespace iris::ast {
	Arg::Arg(std::unique_ptr<Expr> expr)
		: _expr{ std::move(expr) } {
	}

	Arg::Arg(Identifier const &identifier, std::unique_ptr<Expr> expr)
		: _identifier{ identifier }
		, _expr{ std::move(expr) } {
	}

	auto Arg::is_named() const -> bool {
		return _identifier.has_value();
	}

	auto Arg::span() const -> Span {
		if (is_named()) {
			return Span::join(identifier().span(), _expr->span());
		}

		return _expr->span();
	}

	auto Arg::identifier() const -> Identifier {
		try {
			return _identifier.value();
		} catch (std::exception &e) {
			throw std::runtime_error{ "Arg::identifier() : has no value" };
		}
	}

	auto Arg::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}

	CallExpr::CallExpr(Span const &span, std::unique_ptr<Expr> fn, std::vector<Arg> args)
		: _span{ span }
		, _fn{ std::move(fn) }
		, _args{ std::move(args) } {
	}

	auto CallExpr::span() const -> Span {
		return _span;
	}

	auto CallExpr::fn() -> std::unique_ptr<Expr> & {
		return _fn;
	}

	auto CallExpr::args() -> std::vector<Arg> & {
		return _args;
	}

	auto CallExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}