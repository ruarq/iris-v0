//
// Created by ruarq on 18/07/2024.
//

#include "Tokenizer.hpp"

#include <unordered_map>

namespace iris {
	auto Tokenizer::get_kind(std::string_view const literal) -> Token::Kind {
		using enum Token::Kind;
		if (std::unordered_map<std::string_view, Token::Kind> const map{
				{ "let",	 Let	 },
				{ "mut",	 Mut	 },
				{ "fn",		Fn	   },
				{ "return", Return },
				{ "true",	  LBool	},
				{ "false",  LBool	 },
				{ "i8",		Ti8	},
				{ "i16",	 Ti16	  },
				{ "i32",	 Ti32	  },
				{ "i64",	 Ti64	  },
				{ "u8",		Tu8	},
				{ "u16",	 Tu16	  },
				{ "u32",	 Tu32	  },
				{ "u64",	 Tu64	  },
				{ "isz",	 Tisz	  },
				{ "usz",	 Tusz	  },
				{ "bool",	  Tbool	},
				{ "if",		If	   },
				{ "elif",	  Elif   },
				{ "else",	  Else   },
				{ "while",  While	 },
		};
			map.contains(literal)) {
			return map.at(literal);
		}

		return Identifier;
	}

	auto Tokenizer::is_digit(char const c) -> bool {
		return c >= '0' && c <= '9';
	}

	auto Tokenizer::is_identifier(char const c) -> bool {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_') || is_digit(c);
	}

	Tokenizer::Tokenizer(Scanner &scanner, Diagnostics &diagnostics)
		: _scanner{ scanner }
		, _diagnostics{ diagnostics } {
	}

	auto Tokenizer::next() const -> Token {
		using enum Token::Kind;

		while (!_scanner.is_eof()) {
			switch (_scanner.current()) {
				case ' ':
				case '\n':
				case '\t': _scanner.advance(); continue;

				case '#': {
					while (!_scanner.is_eof() && _scanner.current() != '\n') {
						_scanner.advance();
					}
					break;
				}

				case '(': return simple_token(LParen);
				case ')': return simple_token(RParen);
				case '{': return simple_token(LCurly);
				case '}': return simple_token(RCurly);
				case '[': return simple_token(LBracket);
				case ']': return simple_token(RBracket);
				case ':': return simple_token(Colon);
				case ';': return simple_token(Semicolon);
				case ',': return simple_token(Comma);
				case '+': return simple_token(Plus);
				case '-': return simple_token(Minus);
				case '*': return simple_token(Asterisk);
				case '/': return simple_token(Slash);
				case '%': return simple_token(Percent);
				case '@': return simple_token(At);

				case '=': {
					if (_scanner.peek(1) == '=') {
						return simple_token(EqualEqual, 2);
					}
					if (_scanner.peek(1) == '>') {
						return simple_token(Pipe, 2);
					}
					return simple_token(Equal);
				}

				case '!': {
					if (_scanner.peek(1) == '=') {
						return simple_token(ExclamEqual, 2);
					}
					return simple_token(Exclam);
				}

				case '<': {
					if (_scanner.peek(1) == '=') {
						return simple_token(LAngleEqual, 2);
					}
					return simple_token(LAngle);
				}

				case '>': {
					if (_scanner.peek(1) == '=') {
						return simple_token(RAngleEqual, 2);
					}
					return simple_token(RAngle);
				}

				case '0' ... '9': return read_number();

				case 'A' ... 'Z':
				case 'a' ... 'z':
				case '_': return read_keyword_or_identifier();

				case '|': {
					// if (_scanner.peek(1) == '>') {
					//	return simple_token(Pipe, 2);
					// 	}
				}
					[[fallthrough]];

				default:
					_diagnostics.emit(_scanner.span(), "UnknownToken", _scanner.current());
					return simple_token(Unknown);
			}
		}

		return simple_token(EndOfFile);
	}

	auto Tokenizer::scanner() const -> Scanner & {
		return _scanner;
	}

	auto Tokenizer::read_number() const -> Token {
		auto const first = token_begin();
		while (is_digit(_scanner.current())) {
			_scanner.advance();
		}
		auto const last = token_end();
		auto const span = Span::join(first, last);
		return Token{ Token::Kind::LInt, span };
	}

	auto Tokenizer::read_keyword_or_identifier() const -> Token {
		auto const first = token_begin();
		while (is_identifier(_scanner.current())) {
			_scanner.advance();
		}
		auto const last = token_end();
		auto const span = Span::join(first, last);
		return Token{ get_kind(span.string(_scanner.file())), span };
	}

	auto Tokenizer::simple_token(Token::Kind const kind, usz const size) const -> Token {
		auto span = _scanner.span();
		span.set_size(size);
		_scanner.advance(size);
		return Token{ kind, span };
	}

	auto Tokenizer::token_begin() const -> Span {
		return _scanner.span();
	}

	auto Tokenizer::token_end() const -> Span {
		return _scanner.span().set_size(0);
	}
}