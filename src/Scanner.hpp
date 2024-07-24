//
// Created by ruarq on 18/07/2024.
//

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "File.hpp"
#include "Span.hpp"
#include "common.hpp"

namespace iris {
	class Scanner {
	public:
		explicit Scanner(File const &file);

	public:
		[[nodiscard]] auto current() const -> char;
		[[nodiscard]] auto peek(usz amount) const -> char;
		[[nodiscard]] auto consume() -> char;
		auto advance(usz amount = 1) -> void;

		/**
		 * @return The current location of the scanner
		 */
		[[nodiscard]] auto span() const -> Span;
		[[nodiscard]] auto file() const -> File const &;

		[[nodiscard]] auto is_eof() const -> bool;

	private:
		auto update_span(char c) -> void;

	private:
		File const &_file;
		usz _current;
		Span _span;
	};
}	 // iris

#endif	  //SCANNER_HPP
