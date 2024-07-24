//
// Created by ruarq on 18/07/2024.
//

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "Diagnostics.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

namespace iris {
	class Tokenizer {
	public:
		[[nodiscard]] static auto get_kind(std::string_view literal) -> Token::Kind;

		[[nodiscard]] static auto is_digit(char c) -> bool;
		[[nodiscard]] static auto is_identifier(char c) -> bool;

	public:
		Tokenizer(Scanner &scanner, Diagnostics &diagnostics);

	public:
		[[nodiscard]] auto next() const -> Token;
		[[nodiscard]] auto scanner() const -> Scanner &;

	private:
		[[nodiscard]] auto read_number() const -> Token;
		[[nodiscard]] auto read_keyword_or_identifier() const -> Token;
		[[nodiscard]] auto simple_token(Token::Kind kind, usz size = 1) const -> Token;

		[[nodiscard]] auto token_begin() const -> Span;
		[[nodiscard]] auto token_end() const -> Span;

	private:
		Scanner &_scanner;
		Diagnostics &_diagnostics;
	};
}

#endif	  //TOKENIZER_HPP
