//
// Created by ruarq on 18/07/2024.
//

#ifndef LETSTMT_HPP
#define LETSTMT_HPP

#include "Expr.hpp"
#include "Identifier.hpp"
#include "Stmt.hpp"
#include "TypeName.hpp"

namespace iris::ast {
	IRIS_STMT(LetStmt) {
	public:
		LetStmt(Span const &span,
			Identifier const &identifier,
			TypeName type_name,
			std::unique_ptr<Expr> expr);

	public:
		[[nodiscard]] auto span() const->Span override;
		[[nodiscard]] auto identifier() const->Identifier;
		auto type_name()->TypeName &;
		auto expr()->std::unique_ptr<Expr> &;

	private:
		Span _span;
		Identifier _identifier;
		TypeName _type_name;
		std::unique_ptr<Expr> _expr;
	};
}

#endif	  //LETSTMT_HPP
