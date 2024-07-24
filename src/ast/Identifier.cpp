//
// Created by ruarq on 18/07/2024.
//

#include "Identifier.hpp"

namespace iris::ast {
	Identifier::Identifier(Span const &span)
		: _span{ span } {
	}

	auto Identifier::span() const -> Span {
		return _span;
	}

}