//
// Created by ruarq on 17/07/2024.
//

// TODO(ruarq): Add documentation.

#ifndef SPAN_HPP
#define SPAN_HPP

#include <string_view>

#include "File.hpp"
#include "common.hpp"

namespace iris {
	class Span {
	public:
		[[nodiscard]] static auto join(Span const &a, Span const &b) -> Span;

	public:
		Span() = default;
		Span(usz offset, usz size, usz line, usz column);

	public:
		[[nodiscard]] auto make_line(File const &file) const -> Span;

		[[nodiscard]] auto size() const -> usz;

		[[nodiscard]] auto line() const -> usz;
		[[nodiscard]] auto column() const -> usz;

		[[nodiscard]] auto string(std::string_view string) const -> std::string_view;
		[[nodiscard]] auto string(File const &file) const -> std::string_view;

		[[nodiscard]] auto to_string() const -> std::string;

		auto set_size(usz size) -> Span &;

		auto advance() -> void;
		auto advance_newline() -> void;

	private:
		[[nodiscard]] auto end() const -> usz;

	private:
		usz _offset = 0;
		usz _size = 1;
		usz _line = 1;
		usz _column = 1;
	};
}	 // iris

#endif	  //SPAN_HPP
