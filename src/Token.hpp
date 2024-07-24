//
// Created by ruarq on 17/07/2024.
//

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Span.hpp"

namespace iris {
	class Token {
	public:
		enum class Kind {
			LParen,		  ///< (
			RParen,		  ///< )
			LCurly,		  ///< {
			RCurly,		  ///< }
			LBracket,	  ///< [
			RBracket,	  ///< ]
			Colon,		  ///< :
			Semicolon,	  ///< ;
			Comma,		  ///< ,
			At,			  ///< @

			Pipe,	 ///< |>

			LAngle,			///< <
			RAngle,			///< >
			LAngleEqual,	///< <=
			RAngleEqual,	///< >=
			EqualEqual,		///< ==
			ExclamEqual,	///< !=
			Exclam,			///< !

			Equal,	  ///< =

			Plus,		 ///< +
			Minus,		 ///< -
			Asterisk,	 ///< *
			Slash,		 ///< /
			Percent,	 ///< %

			Let,	   ///< let
			Mut,	   ///< mut
			Fn,		   ///< fn
			Return,	   ///< return
			If,		   ///< if
			Elif,	   ///< elif
			Else,	   ///< else
			While,	   ///< while

			LInt,	  ///< integer literal
			LBool,	  ///< boolean literal

			Ti8,	  ///< i8
			Ti16,	  ///< i16
			Ti32,	  ///< i32
			Ti64,	  ///< i64
			Tu8,	  ///< u8
			Tu16,	  ///< u16
			Tu32,	  ///< u32
			Tu64,	  ///< u64
			Tisz,	  ///< isz
			Tusz,	  ///< usz
			Tbool,	  ///< bool

			Identifier,

			Unknown,
			EndOfFile,
		};

	public:
		Token(Kind kind, Span const &span);

	public:
		[[nodiscard]] auto kind() const -> Kind;
		[[nodiscard]] auto span() const -> Span;

	private:
		Kind _kind;
		Span _span;
	};

	[[nodiscard]] auto to_string(Token::Kind kind) -> std::string_view;
}	 // iris

#endif	  //TOKEN_HPP
