//
// Created by ruarq on 22/07/2024.
//

#include "IfStmt.hpp"

namespace iris::ast {
	IfStmt::IfStmt(Span const &span,
		std::unique_ptr<Expr> condition,
		Block then,
		std::vector<ElifStmt> elifs,
		std::optional<Block> orelse)
		: _span{ span }
		, _condition{ std::move(condition) }
		, _then{ std::move(then) }
		, _elifs{ std::move(elifs) }
		, _orelse{ std::move(orelse) } {
	}

	auto IfStmt::span() const -> Span {
		return _span;
	}

	auto IfStmt::condition() -> std::unique_ptr<Expr> & {
		return _condition;
	}

	auto IfStmt::then() -> Block & {
		return _then;
	}

	auto IfStmt::elifs() -> std::vector<ElifStmt> & {
		return _elifs;
	}

	auto IfStmt::has_orelse() const -> bool {
		return _orelse.has_value();
	}

	auto IfStmt::orelse() -> Block & {
		return _orelse.value();
	}

}