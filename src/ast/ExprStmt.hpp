//
// Created by ruarq on 18/07/2024.
//

#ifndef EXPRSTMT_HPP
#define EXPRSTMT_HPP

#include "Expr.hpp"
#include "Stmt.hpp"

namespace iris::ast {
	IRIS_STMT(ExprStmt) {
	public:
		explicit ExprStmt(Span const &span, std::unique_ptr<Expr> expr);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto expr()->std::unique_ptr<Expr> &;

	private:
		Span _span;
		std::unique_ptr<Expr> _expr;
	};
}

#endif	  //EXPRSTMT_HPP
