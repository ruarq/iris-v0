//
// Created by ruarq on 23/07/2024.
//

#ifndef PIPEEXPR_HPP
#define PIPEEXPR_HPP

#include "CallExpr.hpp"
#include "Expr.hpp"

namespace iris::ast {
	IRIS_EXPR(PipeExpr) {
	public:
		PipeExpr(std::unique_ptr<Expr> value, std::unique_ptr<Expr> target);

	public:
		[[nodiscard]] auto span() const->Span override;

		auto value()->std::unique_ptr<Expr> &;
		auto target()->std::unique_ptr<Expr> &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		std::unique_ptr<Expr> _value;
		std::unique_ptr<Expr> _target;
		std::unique_ptr<types::Type> _type;
	};
}

#endif	  //PIPEEXPR_HPP
