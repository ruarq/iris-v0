//
// Created by ruarq on 23/07/2024.
//

#include "WhileStmt.hpp"

namespace iris::ast {
	WhileStmt::WhileStmt(Span const &span, std::unique_ptr<Expr> condition, Block block)
		: _span{ span }
		, _condition{ std::move(condition) }
		, _block{ std::move(block) } {
	}

	auto WhileStmt::span() const -> Span {
		return _span;
	}

	auto WhileStmt::condition() -> std::unique_ptr<Expr> & {
		return _condition;
	}

	auto WhileStmt::block() -> Block & {
		return _block;
	}

}