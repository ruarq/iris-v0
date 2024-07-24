//
// Created by ruarq on 18/07/2024.
//

#ifndef ASSIGNEXPR_HPP
#define ASSIGNEXPR_HPP

#include "Expr.hpp"

namespace iris::ast {

	IRIS_EXPR(AssignExpr) {
	public:
		AssignExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> value);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto target()->std::unique_ptr<Expr> &;
		auto value()->std::unique_ptr<Expr> &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		std::unique_ptr<Expr> _target;
		std::unique_ptr<Expr> _value;
		std::unique_ptr<types::Type> _type;
	};
}

#endif	  //ASSIGNEXPR_HPP
