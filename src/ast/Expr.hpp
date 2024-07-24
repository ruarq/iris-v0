//
// Created by ruarq on 18/07/2024.
//

#ifndef EXPR_HPP
#define EXPR_HPP

#include "Node.hpp"
#include "types/Type.hpp"

namespace iris::ast {
	class Expr : public Node {
	public:
		virtual auto type() -> std::unique_ptr<types::Type> & = 0;
	};
}

#endif	  //EXPR_HPP
