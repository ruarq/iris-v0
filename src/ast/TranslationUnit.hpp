//
// Created by ruarq on 18/07/2024.
//

#ifndef TRANSLATIONUNIT_HPP
#define TRANSLATIONUNIT_HPP

#include "Decl.hpp"
#include "Environment.hpp"
#include "Node.hpp"

namespace iris::ast {

	IRIS_NODE(TranslationUnit) {
	public:
		explicit TranslationUnit(std::vector<std::unique_ptr<Decl>> decls);

	public:
		[[nodiscard]] auto span() const->Span override;
		auto decls()->std::vector<std::unique_ptr<Decl>> &;
		auto environment()->Environment &;

	private:
		std::vector<std::unique_ptr<Decl>> _decls;
		Environment _environment;
	};

}	 // iris

#endif	  //TRANSLATIONUNIT_HPP
