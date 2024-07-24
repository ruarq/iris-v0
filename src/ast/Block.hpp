//
// Created by ruarq on 18/07/2024.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Node.hpp"
#include "Stmt.hpp"

namespace iris::ast {
	IRIS_NODE(Block) {
	public:
		Block(Span const &span, std::vector<std::unique_ptr<Stmt>> stmts);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto stmts()->std::vector<std::unique_ptr<Stmt>> &;

	private:
		Span _span;
		std::vector<std::unique_ptr<Stmt>> _stmts;
	};
}

#endif	  //BLOCK_HPP
