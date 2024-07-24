//
// Created by ruarq on 18/07/2024.
//

#include "LetStmt.hpp"

namespace iris::ast {
	LetStmt::LetStmt(Span const &span,
		Identifier const &identifier,
		TypeName type_name,
		std::unique_ptr<Expr> expr)
		: _span{ span }
		, _identifier{ identifier }
		, _type_name{ std::move(type_name) }
		, _expr{ std::move(expr) } {
	}

	auto LetStmt::span() const -> Span {
		return _span;
	}

	auto LetStmt::identifier() const -> Identifier {
		return _identifier;
	}

	auto LetStmt::type_name() -> TypeName & {
		return _type_name;
	}

	auto LetStmt::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}

}