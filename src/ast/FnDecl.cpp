//
// Created by ruarq on 18/07/2024.
//

#include "FnDecl.hpp"

namespace iris::ast {
	Param::Param(Span const &span, std::vector<Identifier> identifiers, TypeName type_name)
		: _span{ span }
		, _identifiers{ std::move(identifiers) }
		, _type_name{ std::move(type_name) } {
	}

	auto Param::span() const -> Span {
		return _span;
	}

	auto Param::identifiers() -> std::vector<Identifier> & {
		return _identifiers;
	}

	auto Param::type_name() -> TypeName & {
		return _type_name;
	}

	FnDecl::FnDecl(Span const &span,
		const Identifier &identifier,
		std::vector<Param> params,
		TypeName type_name,
		Block block)
		: _span{ span }
		, _identifier{ identifier }
		, _params{ std::move(params) }
		, _type_name{ std::move(type_name) }
		, _block{ std::move(block) } {
	}

	auto FnDecl::span() const -> Span {
		return _span;
	}

	auto FnDecl::identifier() const -> Identifier {
		return _identifier;
	}

	auto FnDecl::params() -> std::vector<Param> & {
		return _params;
	}

	auto FnDecl::type_name() -> TypeName & {
		return _type_name;
	}

	auto FnDecl::block() -> Block & {
		return _block;
	}

	auto FnDecl::environment() -> Environment & {
		return _environment;
	}

}