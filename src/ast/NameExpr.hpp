//
// Created by ruarq on 18/07/2024.
//

#ifndef NAMEEXPR_HPP
#define NAMEEXPR_HPP

#include "Expr.hpp"
#include "Identifier.hpp"

namespace iris::ast {
	IRIS_EXPR(NameExpr) {
	public:
		explicit NameExpr(Identifier const &identifier);

	public:
		[[nodiscard]] auto span() const->Span override;
		[[nodiscard]] auto identifier() const->Identifier;
		auto type()->std::unique_ptr<types::Type> &override;

	private:
		Identifier _identifier;
		std::unique_ptr<types::Type> _type;
	};
}	 // iris

#endif	  //NAMEEXPR_HPP
