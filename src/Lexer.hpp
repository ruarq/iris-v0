//
// Created by ruarq on 18/07/2024.
//

#ifndef LEXER_HPP
#define LEXER_HPP

#include <deque>

#include "Token.hpp"
#include "Tokenizer.hpp"

namespace iris {
	class Lexer {
	public:
		explicit Lexer(Tokenizer &tokenizer);

	public:
		[[nodiscard]] auto peek(usz amount) -> Token;
		[[nodiscard]] auto current() -> Token;
		auto consume() -> Token;

		[[nodiscard]] auto is_eof() -> bool;

		[[nodiscard]] auto tokenizer() const -> Tokenizer &;

	private:
		auto read_into_buffer(usz amount) -> void;
		auto fill_buffer(usz max_size) -> void;

	private:
		Tokenizer &_tokenizer;
		std::deque<Token> _buffer;
	};
}

#endif	  //LEXER_HPP
