//
// Created by ruarq on 18/07/2024.
//

#include "AssignExpr.hpp"

namespace iris::ast {
	AssignExpr::AssignExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> value)
		: _target{ std::move(target) }
		, _value{ std::move(value) } {
	}

	auto AssignExpr::span() const -> Span {
		return Span::join(_target->span(), _value->span());
	}

	auto AssignExpr::target() -> std::unique_ptr<Expr> & {
		return _target;
	}

	auto AssignExpr::value() -> std::unique_ptr<Expr> & {
		return _value;
	}

	auto AssignExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}