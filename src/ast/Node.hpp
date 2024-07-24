//
// Created by ruarq on 18/07/2024.
//

#ifndef NODE_HPP
#define NODE_HPP

#include "Span.hpp"
#include "Visitor.hpp"

#define IRIS_NODE(x) class x : public Visitable<Node, x>
#define IRIS_DECL(x) class x : public Visitable<Decl, x>
#define IRIS_STMT(x) class x : public Visitable<Stmt, x>
#define IRIS_EXPR(x) class x : public Visitable<Expr, x>

namespace iris::ast {
	class Node {
	public:
		virtual ~Node() = default;

	public:
		virtual auto accept(Visitor &visitor) -> void = 0;
		[[nodiscard]] virtual auto span() const -> Span = 0;
	};

	template<typename TBaseNode, typename TNode>
	struct Visitable : public TBaseNode {
		auto accept(Visitor &visitor) -> void final {
			visitor.visit(*static_cast<TNode *>(this));
		}
	};
}	 // iris

#endif	  //NODE_HPP
