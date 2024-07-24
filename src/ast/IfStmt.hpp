//
// Created by ruarq on 22/07/2024.
//

#ifndef IFSTMT_HPP
#define IFSTMT_HPP

#include <optional>

#include "Block.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

namespace iris::ast {
	IRIS_STMT(IfStmt) {
	public:
		using ElifStmt = std::unique_ptr<IfStmt>;

	public:
		IfStmt(Span const &span,
			std::unique_ptr<Expr> condition,
			Block then,
			std::vector<ElifStmt> elifs,
			std::optional<Block> orelse);

	public:
		[[nodiscard]] auto span() const->Span override;

		auto condition()->std::unique_ptr<Expr> &;
		auto then()->Block &;

		auto elifs()->std::vector<ElifStmt> &;

		[[nodiscard]] auto has_orelse() const->bool;
		auto orelse()->Block &;

	private:
		Span _span;
		std::unique_ptr<Expr> _condition;
		Block _then;
		std::vector<ElifStmt> _elifs;
		std::optional<Block> _orelse;
	};
}

#endif	  //IFSTMT_HPP
