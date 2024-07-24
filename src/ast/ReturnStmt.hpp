//
// Created by ruarq on 18/07/2024.
//

#ifndef RETURNSTMT_HPP
#define RETURNSTMT_HPP

#include "Expr.hpp"
#include "Stmt.hpp"

namespace iris::ast {
	IRIS_STMT(ReturnStmt) {
	public:
		ReturnStmt(Span const &span, std::unique_ptr<Expr> expr);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto expr()->std::unique_ptr<Expr> &;

	private:
		Span _span;
		std::unique_ptr<Expr> _expr;
	};
}

#endif	  //RETURNSTMT_HPP
