//
// Created by ruarq on 18/07/2024.
//

#include "ReturnStmt.hpp"
namespace iris::ast {
	ReturnStmt::ReturnStmt(Span const &span, std::unique_ptr<Expr> expr)
		: _span{ span }
		, _expr{ std::move(expr) } {
	}

	auto ReturnStmt::span() const -> Span {
		return _span;
	}

	auto ReturnStmt::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}

}