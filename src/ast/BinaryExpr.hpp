//
// Created by ruarq on 18/07/2024.
//

#ifndef BINARYEXPR_HPP
#define BINARYEXPR_HPP

#include "Expr.hpp"

namespace iris::ast {
	IRIS_EXPR(BinaryExpr) {
	public:
		enum class Op {
			Add,
			Sub,
			Mul,
			Div,
			Mod,
			Less,
			Greater,
			LessEq,
			GreaterEq,
			Eq,
			NotEq,
		};

	public:
		BinaryExpr(std::unique_ptr<Expr> left, Op op, std::unique_ptr<Expr> right);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto left()->std::unique_ptr<Expr> &;
		[[nodiscard]] auto op() const->Op;
		auto right()->std::unique_ptr<Expr> &;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		std::unique_ptr<Expr> _left;
		Op _op;
		std::unique_ptr<Expr> _right;
		std::unique_ptr<types::Type> _type;
	};

	[[nodiscard]] auto to_string(BinaryExpr::Op op) -> std::string_view;
}

#endif	  //BINARYEXPR_HPP
