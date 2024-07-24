//
// Created by ruarq on 19/07/2024.
//

#include "TypeName.hpp"

namespace iris::ast {
	TypeName::TypeName(Span const &span, std::unique_ptr<types::Type> type)
		: _span{ span }
		, _type{ std::move(type) } {
	}

	auto TypeName::span() const -> Span {
		return _span;
	}

	auto TypeName::type() -> std::unique_ptr<types::Type> & {
		return _type;
	}

}