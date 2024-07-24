//
// Created by ruarq on 18/07/2024.
//

#include "Block.hpp"

namespace iris::ast {
	Block::Block(Span const &span, std::vector<std::unique_ptr<Stmt>> stmts)
		: _span{ span }
		, _stmts{ std::move(stmts) } {
	}

	auto Block::span() const -> Span {
		return _span;
	}

	auto Block::stmts() -> std::vector<std::unique_ptr<Stmt>> & {
		return _stmts;
	}

}