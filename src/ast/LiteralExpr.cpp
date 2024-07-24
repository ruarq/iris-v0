//
// Created by ruarq on 18/07/2024.
//

#include "LiteralExpr.hpp"

namespace iris::ast {
	LiteralExpr::LiteralExpr(Span const &span, Value value)
		: _span{ span }
		, _value{ std::move(value) } {
	}

	auto LiteralExpr::span() const -> Span {
		return _span;
	}

	auto LiteralExpr::value() -> Value & {
		return _value;
	}

	auto LiteralExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}