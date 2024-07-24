//
// Created by ruarq on 18/07/2024.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP

namespace iris::ast {
	// Nodes
	class Block;
	class TranslationUnit;

	// Declarations
	class FnDecl;

	// Statements
	class ReturnStmt;
	class ExprStmt;
	class LetStmt;
	class MutStmt;
	class IfStmt;
	class WhileStmt;

	// Expressions
	class AssignExpr;
	class BinaryExpr;
	class NameExpr;
	class LiteralExpr;
	class GroupExpr;
	class CallExpr;
	class PipeExpr;

#define VISIT(x) virtual auto visit(x &) -> void = 0

	class Visitor {
	public:
		virtual ~Visitor() = default;

	public:
		VISIT(TranslationUnit);
		VISIT(Block);

		VISIT(FnDecl);

		VISIT(ReturnStmt);
		VISIT(ExprStmt);
		VISIT(MutStmt);
		VISIT(LetStmt);
		VISIT(IfStmt);
		VISIT(WhileStmt);

		VISIT(AssignExpr);
		VISIT(NameExpr);
		VISIT(BinaryExpr);
		VISIT(LiteralExpr);
		VISIT(CallExpr);
		VISIT(GroupExpr);
		VISIT(PipeExpr);
	};

#undef VISIT

}	 // iris

#endif	  //VISITOR_HPP
