//
// Created by ruarq on 19/07/2024.
//

#include "GroupExpr.hpp"

namespace iris::ast {
	GroupExpr::GroupExpr(Span const &span, std::unique_ptr<Expr> expr)
		: _span{ span }
		, _expr{ std::move(expr) } {
	}

	auto GroupExpr::span() const -> Span {
		return _span;
	}

	auto GroupExpr::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}

	auto GroupExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}