//
// Created by ruarq on 23/07/2024.
//

#include "PipeExpr.hpp"

namespace iris::ast {
	PipeExpr::PipeExpr(std::unique_ptr<Expr> value, std::unique_ptr<Expr> target)
		: _value{ std::move(value) }
		, _target{ std::move(target) } {
	}

	auto PipeExpr::span() const -> Span {
		return Span::join(_value->span(), _target->span());
	}

	auto PipeExpr::value() -> std::unique_ptr<Expr> & {
		return _value;
	}

	auto PipeExpr::target() -> std::unique_ptr<Expr> & {
		return _target;
	}

	auto PipeExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}