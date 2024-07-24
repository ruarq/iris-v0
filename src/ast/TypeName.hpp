//
// Created by ruarq on 18/07/2024.
//

#ifndef TYPENODE_HPP
#define TYPENODE_HPP

#include "Span.hpp"
#include "types/Type.hpp"

namespace iris::ast {
	class TypeName {
	public:
		TypeName(Span const &span, std::unique_ptr<types::Type> type);

	public:
		[[nodiscard]] auto span() const -> Span;
		auto type() -> std::unique_ptr<types::Type> &;

	private:
		Span _span;
		std::unique_ptr<types::Type> _type;
	};
}	 // iris

#endif	  //TYPENODE_HPP
