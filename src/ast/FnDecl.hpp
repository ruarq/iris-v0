//
// Created by ruarq on 18/07/2024.
//

#ifndef FNDECL_HPP
#define FNDECL_HPP

#include "Block.hpp"
#include "Decl.hpp"
#include "Environment.hpp"
#include "Identifier.hpp"
#include "TypeName.hpp"

namespace iris::ast {
	class Param {
	public:
		Param(Param const &) = delete;
		Param(Param &&) = default;
		Param(Span const &span, std::vector<Identifier> identifiers, TypeName type_name);

	public:
		[[nodiscard]] auto span() const -> Span;
		auto identifiers() -> std::vector<Identifier> &;
		auto type_name() -> TypeName &;

	private:
		Span _span;
		std::vector<Identifier> _identifiers;
		TypeName _type_name;
	};

	IRIS_DECL(FnDecl) {
	public:
		FnDecl(Span const &span,
			Identifier const &identifier,
			std::vector<Param> params,
			TypeName type_name,
			Block block);

	public:
		[[nodiscard]] auto span() const->Span override;
		[[nodiscard]] auto identifier() const->Identifier;
		auto params()->std::vector<Param> &;
		auto type_name()->TypeName &;
		auto block()->Block &;
		auto environment()->Environment &;

	private:
		Span _span;
		Identifier _identifier;
		std::vector<Param> _params;
		TypeName _type_name;
		Block _block;
		Environment _environment;
	};
}

#endif	  //FNDECL_HPP
