//
// Created by ruarq on 18/07/2024.
//

#include "ExprStmt.hpp"

namespace iris::ast {
	ExprStmt::ExprStmt(Span const &span, std::unique_ptr<Expr> expr)
		: _span{ span }
		, _expr{ std::move(expr) } {
	}

	auto ExprStmt::span() const -> Span {
		return _span;
	}

	auto ExprStmt::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}
}