//
// Created by ruarq on 18/07/2024.
//

#ifndef AST_HPP
#define AST_HPP

// Nodes
#include "Block.hpp"
#include "Decl.hpp"
#include "Expr.hpp"
#include "Node.hpp"
#include "Stmt.hpp"
#include "TranslationUnit.hpp"

// Declarations
#include "FnDecl.hpp"

// Statements
#include "ExprStmt.hpp"
#include "IfStmt.hpp"
#include "LetStmt.hpp"
#include "MutStmt.hpp"
#include "ReturnStmt.hpp"
#include "WhileStmt.hpp"

// Expressions
#include "AssignExpr.hpp"
#include "BinaryExpr.hpp"
#include "CallExpr.hpp"
#include "GroupExpr.hpp"
#include "LiteralExpr.hpp"
#include "NameExpr.hpp"
#include "PipeExpr.hpp"

// Types
#include "TypeName.hpp"

// Other
#include "Identifier.hpp"
#include "Value.hpp"
#include "Visitor.hpp"

#endif	  //AST_HPP
