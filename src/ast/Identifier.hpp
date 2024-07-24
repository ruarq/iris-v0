//
// Created by ruarq on 18/07/2024.
//

#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include "Span.hpp"

namespace iris::ast {
	class Identifier {
	public:
		explicit Identifier(Span const &span);

	public:
		[[nodiscard]] auto span() const -> Span;

	private:
		Span _span;
	};
}

#endif	  //IDENTIFIER_HPP
