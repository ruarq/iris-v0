//
// Created by ruarq on 23/07/2024.
//

#ifndef WHILESTMT_HPP
#define WHILESTMT_HPP

#include "Block.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

namespace iris::ast {
	IRIS_STMT(WhileStmt) {
	public:
		WhileStmt(Span const &span, std::unique_ptr<Expr> condition, Block block);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto condition()->std::unique_ptr<Expr> &;
		auto block()->Block &;

	private:
		Span _span;
		std::unique_ptr<Expr> _condition;
		Block _block;
	};
}

#endif	  //WHILESTMT_HPP
