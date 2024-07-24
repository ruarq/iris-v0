//
// Created by ruarq on 18/07/2024.
//

#ifndef LITERALEXPR_HPP
#define LITERALEXPR_HPP

#include "Expr.hpp"
#include "Value.hpp"

namespace iris::ast {
	IRIS_EXPR(LiteralExpr) {
	public:
		LiteralExpr(Span const &span, Value value);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto value()->Value &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		Span _span;
		Value _value;
		std::unique_ptr<types::Type> _type;
	};
}

#endif	  //LITERALEXPR_HPP
