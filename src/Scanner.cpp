//
// Created by ruarq on 18/07/2024.
//

#include "Scanner.hpp"

namespace iris {
	Scanner::Scanner(File const &file)
		: _file{ file }
		, _current{ 0 } {
	}

	auto Scanner::current() const -> char {
		return peek(0);
	}

	auto Scanner::peek(usz const amount) const -> char {
		if (_current + amount >= _file.content().size()) {
			return '\0';
		}

		return _file.content()[_current + amount];
	}

	auto Scanner::consume() -> char {
		auto const current = this->current();
		advance();
		return current;
	}

	auto Scanner::advance(usz amount) -> void {
		for (; amount > 0; --amount) {
			++_current;
			update_span(current());
		}
	}

	auto Scanner::span() const -> Span {
		return _span;
	}
	auto Scanner::file() const -> File const & {
		return _file;
	}

	auto Scanner::update_span(char const c) -> void {
		if (c == '\n') {
			_span.advance_newline();
		} else {
			_span.advance();
		}
	}

	auto Scanner::is_eof() const -> bool {
		return _current >= _file.content().size();
	}
}	 // iris