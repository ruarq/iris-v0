//
// Created by ruarq on 18/07/2024.
//

#include "Lexer.hpp"

namespace iris {
	Lexer::Lexer(Tokenizer &tokenizer)
		: _tokenizer{ tokenizer } {
	}

	auto Lexer::peek(usz const amount) -> Token {
		fill_buffer(amount + 1);
		return _buffer[amount];
	}

	auto Lexer::current() -> Token {
		return peek(0);
	}

	auto Lexer::consume() -> Token {
		auto const token = current();
		_buffer.pop_front();
		return token;
	}

	auto Lexer::is_eof() -> bool {
		return current().kind() == Token::Kind::EndOfFile;
	}
	auto Lexer::tokenizer() const -> Tokenizer & {
		return _tokenizer;
	}

	auto Lexer::read_into_buffer(usz amount) -> void {
		for (; amount > 0; --amount) {
			_buffer.push_back(_tokenizer.next());
		}
	}

	auto Lexer::fill_buffer(usz const max_size) -> void {
		if (_buffer.size() >= max_size) {
			return;
		}

		read_into_buffer(max_size - _buffer.size());
	}
}