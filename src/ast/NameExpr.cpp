//
// Created by ruarq on 18/07/2024.
//

#include "NameExpr.hpp"

namespace iris::ast {
	NameExpr::NameExpr(Identifier const &identifier)
		: _identifier{ identifier } {
	}

	auto NameExpr::span() const -> Span {
		return _identifier.span();
	}

	auto NameExpr::identifier() const -> Identifier {
		return _identifier;
	}

	auto NameExpr::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}	 // iris