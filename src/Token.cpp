//
// Created by ruarq on 17/07/2024.
//

#include "Token.hpp"

namespace iris {
	Token::Token(Kind const kind, Span const &span)
		: _kind{ kind }
		, _span{ span } {
	}

	auto Token::kind() const -> Kind {
		return _kind;
	}

	auto Token::span() const -> Span {
		return _span;
	}

#define _(x) [static_cast<int>(x)] = #x

	auto to_string(Token::Kind const kind) -> std::string_view {
		using enum Token::Kind;
		constexpr std::string_view table[] = {
			_(LParen),
			_(RParen),
			_(LCurly),
			_(RCurly),
			_(LBracket),
			_(RBracket),
			_(Colon),
			_(Semicolon),
			_(Comma),
			_(At),

			_(Pipe),

			_(LAngle),
			_(RAngle),
			_(LAngleEqual),
			_(RAngleEqual),
			_(EqualEqual),
			_(ExclamEqual),
			_(Exclam),

			_(Equal),

			_(Plus),
			_(Minus),
			_(Asterisk),
			_(Slash),
			_(Percent),

			_(Let),
			_(Mut),
			_(Fn),
			_(Return),
			_(If),
			_(Elif),
			_(Else),
			_(While),

			_(LInt),
			_(LBool),

			_(Ti8),
			_(Ti16),
			_(Ti32),
			_(Ti64),
			_(Tu8),
			_(Tu16),
			_(Tu32),
			_(Tu64),
			_(Tisz),
			_(Tusz),
			_(Tbool),

			_(Identifier),
			_(Unknown),
			_(EndOfFile),
		};

		return table[static_cast<usz>(kind)];
	}

#undef _

}	 // iris