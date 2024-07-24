//
// Created by ruarq on 18/07/2024.
//

#include "TranslationUnit.hpp"

namespace iris::ast {
	TranslationUnit::TranslationUnit(std::vector<std::unique_ptr<Decl>> decls)
		: _decls{ std::move(decls) } {
	}

	auto TranslationUnit::span() const -> Span {
		// TODO(ruarq): Return useful value.
		return Span{};
	}

	auto TranslationUnit::decls() -> std::vector<std::unique_ptr<Decl>> & {
		return _decls;
	}

	auto TranslationUnit::environment() -> Environment & {
		return _environment;
	}

}	 // iris