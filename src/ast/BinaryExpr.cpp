//
// Created by ruarq on 18/07/2024.
//

#include "BinaryExpr.hpp"

namespace iris::ast {
	BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Op op, std::unique_ptr<Expr> right)
		: _left{ std::move(left) }
		, _op{ op }
		, _right{ std::move(right) } {
	}

	auto BinaryExpr::span() const -> Span {
		return Span::join(_left->span(), _right->span());
	}

	auto BinaryExpr::left() -> std::unique_ptr<Expr> & {
		return _left;
	}

	auto BinaryExpr::op() const -> Op {
		return _op;
	}

	auto BinaryExpr::right() -> std::unique_ptr<Expr> & {
		return _right;
	}

	auto BinaryExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

#define _(x) [static_cast<int>(x)] = #x

	auto to_string(BinaryExpr::Op const op) -> std::string_view {
		using enum BinaryExpr::Op;
		constexpr std::string_view table[] = {
			_(Add),
			_(Sub),
			_(Mul),
			_(Div),
			_(Mod),
			_(Less),
			_(Greater),
			_(LessEq),
			_(GreaterEq),
			_(Eq),
			_(NotEq),
		};
		return table[static_cast<int>(op)];
	}

#undef _
}