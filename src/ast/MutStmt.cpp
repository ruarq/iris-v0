//
// Created by ruarq on 18/07/2024.
//

#include "MutStmt.hpp"

namespace iris::ast {
	MutStmt::MutStmt(Span const &span,
		Identifier const &identifier,
		TypeName type_name,
		std::unique_ptr<Expr> expr)
		: _span{ span }
		, _identifier{ identifier }
		, _type_name{ std::move(type_name) }
		, _expr{ std::move(expr) } {
	}

	auto MutStmt::span() const -> Span {
		return _span;
	}

	auto MutStmt::identifier() const -> Identifier {
		return _identifier;
	}

	auto MutStmt::type_name() -> TypeName & {
		return _type_name;
	}

	auto MutStmt::expr() -> std::unique_ptr<Expr> & {
		return _expr;
	}

}