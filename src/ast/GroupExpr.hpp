//
// Created by ruarq on 19/07/2024.
//

#ifndef GROUPEXPR_HPP
#define GROUPEXPR_HPP

#include "Expr.hpp"

namespace iris::ast {
	IRIS_EXPR(GroupExpr) {
	public:
		GroupExpr(Span const &span, std::unique_ptr<Expr> expr);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto expr()->std::unique_ptr<Expr> &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		Span _span;
		std::unique_ptr<Expr> _expr;
		std::unique_ptr<types::Type> _type;
	};
}

#endif	  //GROUPEXPR_HPP
