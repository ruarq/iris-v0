//
// Created by ruarq on 18/07/2024.
//

#include "Parser.hpp"

namespace iris {
	auto Parser::is_decl(Token::Kind const kind) -> bool {
		return kind == Token::Kind::Fn;
	}

	auto Parser::is_stmt(Token::Kind const kind) -> bool {
		return kind == Token::Kind::Let || kind == Token::Kind::Mut ||
			   kind == Token::Kind::Return || kind == Token::Kind::If ||
			   kind == Token::Kind::While || is_expr(kind);
	}

	auto Parser::is_expr(Token::Kind const kind) -> bool {
		return kind == Token::Kind::LInt || kind == Token::Kind::Identifier ||
			   kind == Token::Kind::LParen;
	}

	auto Parser::is_block(Token::Kind const kind) -> bool {
		return kind == Token::Kind::LCurly;
	}

	auto Parser::is_param(Token::Kind const kind) -> bool {
		return kind == Token::Kind::Identifier;
	}

	auto Parser::is_type(Token::Kind const kind) -> bool {
		using enum Token::Kind;
		switch (kind) {
			case Ti8:
			case Ti16:
			case Ti32:
			case Ti64:
			case Tu8:
			case Tu16:
			case Tu32:
			case Tu64:
			case Tisz:
			case Tusz:
			case Tbool: return true;
			default: break;
		}

		return false;
	}

	auto Parser::is_arg(Token::Kind const kind) -> bool {
		return kind == Token::Kind::Identifier || is_expr(kind);
	}

	Parser::Parser(Lexer &lexer, Diagnostics &diagnostics)
		: _lexer{ lexer }
		, _diagnostics{ diagnostics } {
	}

	auto Parser::parse() -> ast::TranslationUnit {
		return parse_translation_unit();
	}

	auto Parser::match(Token::Kind const kind) const -> bool {
		return kind == _lexer.current().kind();
	}

	auto Parser::consume(Token::Kind const kind) const -> Token {
		auto const current = _lexer.current();
		if (current.kind() != kind) {
			_diagnostics.emit(current.span(),
				"UnexpectedTokenExpected",
				current.span().string(_lexer.tokenizer().scanner().file()),
				to_string(kind));
		}

		return _lexer.consume();
	}

	auto Parser::parse_translation_unit() -> ast::TranslationUnit {
		auto decls = parse_decls();
		return ast::TranslationUnit{ std::move(decls) };
	}

	auto Parser::parse_decls() -> std::vector<std::unique_ptr<ast::Decl>> {
		std::vector<std::unique_ptr<ast::Decl>> decls;
		while (!_lexer.is_eof()) {
			decls.push_back(parse_decl());
		}
		return decls;
	}

	auto Parser::parse_decl() -> std::unique_ptr<ast::Decl> {
		return parse_fn_decl();
	}

	auto Parser::parse_fn_decl() -> std::unique_ptr<ast::FnDecl> {
		auto const first = consume(Token::Kind::Fn);
		auto const identifier = parse_identifier();
		consume(Token::Kind::LParen);
		auto params = parse_params();
		auto last = consume(Token::Kind::RParen).span();
		auto type = parse_optional_type();
		if (type.type()) {
			last = type.span();
		} else {
			type.type() = types::make_unit_type();
		}

		auto block = parse_block();
		return std::make_unique<ast::FnDecl>(Span::join(first.span(), last),
			identifier,
			std::move(params),
			std::move(type),
			std::move(block));
	}

	auto Parser::parse_params() -> std::vector<ast::Param> {
		if (!is_param(_lexer.current().kind())) {
			return {};
		}

		std::vector<ast::Param> params;
		params.push_back(parse_param());

		while (match(Token::Kind::Comma)) {
			_lexer.consume();
			params.push_back(parse_param());
		}
		return params;
	}

	auto Parser::parse_param() -> ast::Param {
		auto const first = _lexer.current().span();
		auto identifiers = parse_identifiers();
		consume(Token::Kind::Colon);
		auto type = parse_type_name();
		return ast::Param{ Span::join(first, type.span()),
			std::move(identifiers),
			std::move(type) };
	}

	auto Parser::parse_identifiers() -> std::vector<ast::Identifier> {
		std::vector identifiers{ parse_identifier() };
		while (match(Token::Kind::Comma)) {
			_lexer.consume();
			identifiers.push_back(parse_identifier());
		}
		return identifiers;
	}

	auto Parser::parse_identifier() -> ast::Identifier {
		auto const token = consume(Token::Kind::Identifier);
		return ast::Identifier{ token.span() };
	}

	auto Parser::parse_optional_type() -> ast::TypeName {
		if (_lexer.current().kind() != Token::Kind::Colon) {
			// TODO(ruarq): Return unit type.
			return { _lexer.current().span(), nullptr };
		}

		_lexer.consume();
		return parse_type_name();
	}

	auto Parser::parse_block() -> ast::Block {
		auto const lcurly = consume(Token::Kind::LCurly);
		auto stmts = parse_stmts();
		auto const rcurly = consume(Token::Kind::RCurly);
		return ast::Block{ Span::join(lcurly.span(), rcurly.span()), std::move(stmts) };
	}

	auto Parser::parse_stmts() -> std::vector<std::unique_ptr<ast::Stmt>> {
		std::vector<std::unique_ptr<ast::Stmt>> stmts;
		while (is_stmt(_lexer.current().kind())) {
			stmts.push_back(parse_stmt());
		}
		return stmts;
	}

	auto Parser::parse_stmt() -> std::unique_ptr<ast::Stmt> {
		auto const current = _lexer.current();
		using enum Token::Kind;
		switch (current.kind()) {
			case Let: return parse_let_stmt();
			case Mut: return parse_mut_stmt();
			case Return: return parse_return_stmt();
			case If: return parse_if_stmt();
			case While: return parse_while_stmt();
			default: break;
		}

		return parse_expr_stmt();
	}

	auto Parser::parse_let_stmt() -> std::unique_ptr<ast::LetStmt> {
		auto const first = consume(Token::Kind::Let);
		auto const identifier = parse_identifier();
		auto type = parse_optional_type();
		consume(Token::Kind::Equal);
		auto expr = parse_expr();
		auto const last = consume(Token::Kind::Semicolon);
		return std::make_unique<ast::LetStmt>(Span::join(first.span(), last.span()),
			identifier,
			std::move(type),
			std::move(expr));
	}

	auto Parser::parse_mut_stmt() -> std::unique_ptr<ast::MutStmt> {
		auto const first = consume(Token::Kind::Mut);
		auto const identifier = parse_identifier();
		auto type = parse_optional_type();
		if (type.type() && !match(Token::Kind::Equal)) {
			auto const last = consume(Token::Kind::Semicolon);
			return std::make_unique<ast::MutStmt>(Span::join(first.span(), last.span()),
				identifier,
				std::move(type),
				nullptr);
		}
		consume(Token::Kind::Equal);
		auto expr = parse_expr();
		auto const last = consume(Token::Kind::Semicolon);
		return std::make_unique<ast::MutStmt>(Span::join(first.span(), last.span()),
			identifier,
			std::move(type),
			std::move(expr));
	}

	auto Parser::parse_return_stmt() -> std::unique_ptr<ast::ReturnStmt> {
		auto const first = consume(Token::Kind::Return);
		auto expr = parse_expr();
		auto const last = consume(Token::Kind::Semicolon);
		return std::make_unique<ast::ReturnStmt>(Span::join(first.span(), last.span()),
			std::move(expr));
	}

	auto Parser::parse_expr_stmt() -> std::unique_ptr<ast::ExprStmt> {
		auto expr = parse_expr();
		auto const last = consume(Token::Kind::Semicolon);
		return std::make_unique<ast::ExprStmt>(Span::join(expr->span(), last.span()),
			std::move(expr));
	}

	auto Parser::parse_if_stmt() -> std::unique_ptr<ast::IfStmt> {
		auto const first = consume(Token::Kind::If);

		auto cond = parse_expr();

		auto then = parse_block();
		auto last = then.span();

		auto elifs = parse_elif_stmts();

		std::optional<ast::Block> orelse;
		if (match(Token::Kind::Else)) {
			_lexer.consume();
			orelse = parse_block();
			last = orelse.value().span();
		}

		return std::make_unique<ast::IfStmt>(Span::join(first.span(), last),
			std::move(cond),
			std::move(then),
			std::move(elifs),
			std::move(orelse));
	}

	auto Parser::parse_elif_stmts() -> std::vector<ast::IfStmt::ElifStmt> {
		std::vector<ast::IfStmt::ElifStmt> elifs;
		while (match(Token::Kind::Elif)) {
			elifs.push_back(parse_elif_stmt());
		}
		return elifs;
	}

	auto Parser::parse_elif_stmt() -> ast::IfStmt::ElifStmt {
		auto const first = consume(Token::Kind::Elif);
		auto expr = parse_expr();
		auto block = parse_block();
		return std::make_unique<ast::IfStmt>(Span::join(first.span(), block.span()),
			std::move(expr),
			std::move(block),
			std::vector<ast::IfStmt::ElifStmt>{},
			std::nullopt);
	}

	auto Parser::parse_while_stmt() -> std::unique_ptr<ast::WhileStmt> {
		auto const first = consume(Token::Kind::While);
		auto condition = parse_expr();
		auto block = parse_block();
		return std::make_unique<ast::WhileStmt>(Span::join(first.span(), block.span()),
			std::move(condition),
			std::move(block));
	}

	auto Parser::parse_expr() -> std::unique_ptr<ast::Expr> {
		return parse_assign_expr();
	}

	auto Parser::parse_assign_expr() -> std::unique_ptr<ast::Expr> {
		auto target = parse_equality_expr();
		if (match(Token::Kind::Equal)) {
			_lexer.consume();
			auto value = parse_expr();
			target = std::make_unique<ast::AssignExpr>(std::move(target), std::move(value));
		}
		return target;
	}

	auto Parser::parse_equality_expr() -> std::unique_ptr<ast::Expr> {
		auto left = parse_comparison_expr();
		while (match_any(Token::Kind::EqualEqual, Token::Kind::ExclamEqual)) {
			auto const op = parse_op();
			auto right = parse_comparison_expr();
			left = std::make_unique<ast::BinaryExpr>(std::move(left), op, std::move(right));
		}
		return left;
	}

	auto Parser::parse_comparison_expr() -> std::unique_ptr<ast::Expr> {
		auto left = parse_add_sub_expr();
		using enum Token::Kind;
		while (match_any(LAngle, RAngle, LAngleEqual, RAngleEqual)) {
			auto const op = parse_op();
			auto right = parse_add_sub_expr();
			left = std::make_unique<ast::BinaryExpr>(std::move(left), op, std::move(right));
		}
		return left;
	}

	auto Parser::parse_add_sub_expr() -> std::unique_ptr<ast::Expr> {
		auto left = parse_mul_div_expr();
		while (match_any(Token::Kind::Plus, Token::Kind::Minus)) {
			auto const op = parse_op();
			auto right = parse_mul_div_expr();
			left = std::make_unique<ast::BinaryExpr>(std::move(left), op, std::move(right));
		}
		return left;
	}

	auto Parser::parse_op() -> ast::BinaryExpr::Op {
		using enum Token::Kind;
		using enum ast::BinaryExpr::Op;
		switch (_lexer.consume().kind()) {
			case Plus: return Add;
			case Minus: return Sub;
			case Asterisk: return Mul;
			case Slash: return Div;
			case Percent: return Mod;
			case LAngle: return Less;
			case RAngle: return Greater;
			case LAngleEqual: return LessEq;
			case RAngleEqual: return GreaterEq;
			case EqualEqual: return Eq;
			case ExclamEqual: return NotEq;

			default: throw std::runtime_error{ "This shouldn't happen." };
		}
	}

	auto Parser::parse_mul_div_expr() -> std::unique_ptr<ast::Expr> {
		auto left = parse_pipe_expr();
		using enum Token::Kind;
		while (match_any(Asterisk, Slash, Percent)) {
			auto const op = parse_op();
			auto right = parse_pipe_expr();
			left = std::make_unique<ast::BinaryExpr>(std::move(left), op, std::move(right));
		}
		return left;
	}

	auto Parser::parse_pipe_expr() -> std::unique_ptr<ast::Expr> {
		auto value = parse_call_expr();
		while (match(Token::Kind::Pipe)) {
			_lexer.consume();
			auto target = parse_call_expr();
			value = std::make_unique<ast::PipeExpr>(std::move(value), std::move(target));
		}

		return value;
	}

	auto Parser::parse_call_expr() -> std::unique_ptr<ast::Expr> {
		auto fn = parse_term_expr();
		if (match(Token::Kind::LParen)) {
			_lexer.consume();
			auto args = parse_args();
			auto const last = consume(Token::Kind::RParen);
			return std::make_unique<ast::CallExpr>(Span::join(fn->span(), last.span()),
				std::move(fn),
				std::move(args));
		}
		return fn;
	}

	auto Parser::parse_args() -> std::vector<ast::Arg> {
		if (!is_arg(_lexer.current().kind())) {
			return {};
		}

		bool allow_positional = true;

		std::vector<ast::Arg> args;
		args.push_back(parse_arg(allow_positional));

		while (match(Token::Kind::Comma)) {
			_lexer.consume();
			args.push_back(parse_arg(allow_positional));
		}
		return args;
	}

	auto Parser::parse_arg(bool &allow_positional) -> ast::Arg {
		if (allow_positional && !match_sequence(Token::Kind::Identifier, Token::Kind::Colon)) {
			return ast::Arg{ parse_expr() };
		}

		allow_positional = false;
		auto identifier = parse_identifier();
		consume(Token::Kind::Colon);
		auto expr = parse_expr();
		return ast::Arg{ identifier, std::move(expr) };
	}

	auto Parser::parse_term_expr() -> std::unique_ptr<ast::Expr> {
		using enum Token::Kind;
		switch (_lexer.current().kind()) {
			case LParen: return parse_group_expr();
			case LBool:
			case LInt: return parse_literal_expr();
			case Identifier: return parse_name_expr();
			default:
				// TODO(ruarq): Add ast nodes to handle non-existing expressions.
				throw std::runtime_error{ "Need ast::ErrorExpr" };
		}
	}

	auto Parser::parse_group_expr() -> std::unique_ptr<ast::GroupExpr> {
		auto const first = consume(Token::Kind::LParen);
		auto expr = parse_expr();
		auto const last = consume(Token::Kind::RParen);
		return std::make_unique<ast::GroupExpr>(Span::join(first.span(), last.span()),
			std::move(expr));
	}

	auto Parser::parse_literal_expr() -> std::unique_ptr<ast::LiteralExpr> {
		using enum Token::Kind;
		ast::Value value{ false };
		auto const token = _lexer.consume();
		auto const literal = token.span().string(_lexer.tokenizer().scanner().file());
		switch (token.kind()) {
			case LBool: {
				if (literal == "true") {
					value = ast::Value{ true };
				}
				break;
			}

			case LInt: {
				i64 const num_int = std::stoll(std::string{ literal });
				value = ast::Value{ num_int };
				break;
			}
			default: throw std::runtime_error{ "This shouldn't happen." };
		}

		return std::make_unique<ast::LiteralExpr>(token.span(), value);
	}

	auto Parser::parse_name_expr() -> std::unique_ptr<ast::NameExpr> {
		return std::make_unique<ast::NameExpr>(parse_identifier());
	}

	auto Parser::parse_type_name() -> ast::TypeName {
		auto [span, type] = parse_type();
		return ast::TypeName{ span, std::move(type) };
	}

	template<typename... Ts>
	auto make_type_list(Ts &&...ts) {
		std::vector<std::string> strings;
		(strings.push_back(to_string(static_cast<Ts>(ts))), ...);
		return strings;
	}

	auto Parser::parse_type() -> std::pair<Span, std::unique_ptr<types::Type>> {
		using enum Token::Kind;

		if (!is_type(_lexer.current().kind())) {
			_diagnostics.emit(_lexer.current().span(),
				"UnexpectedTokenExpectedAnyOf",
				_lexer.current().span().string(_lexer.tokenizer().scanner().file()),
				"i8, i16, i32, i64, u8, u16, u32, u64, isz, usz, bool");
			return { _lexer.current().span(), nullptr };
		}

		auto const token = _lexer.consume();
		using enum types::SimpleType::Kind;
		auto make_type = [&](types::SimpleType::Kind const kind) {
			return std::pair{ token.span(), make_simple_type(kind) };
		};
		switch (token.kind()) {
			case Ti8: return make_type(Int8);
			case Ti16: return make_type(Int16);
			case Ti32: return make_type(Int32);
			case Ti64: return make_type(Int64);
			case Tu8: return make_type(UInt8);
			case Tu16: return make_type(UInt16);
			case Tu32: return make_type(UInt32);
			case Tu64: return make_type(UInt64);
			case Tisz: return make_type(ISize);
			case Tusz: return make_type(USize);
			case Tbool: return make_type(Bool);
			default: throw std::runtime_error{ "This shouldn't happen" };
		};
	}

}	 // iris