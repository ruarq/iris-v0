//
// Created by ruarq on 17/07/2024.
//

#include "Span.hpp"

namespace iris {
	auto Span::join(Span const &a, Span const &b) -> Span {
		auto const offset = std::min(a._offset, b._offset);
		auto const end = std::max(a.end(), b.end());

		auto line = a._line;
		auto column = a._column;

		if (a._offset > b._offset) {
			line = b._column;
			column = b._column;
		}

		return Span{ offset, end - offset, line, column };
	}

	Span::Span(usz const offset, usz const size, usz const line, usz const column)
		: _offset{ offset }
		, _size{ size }
		, _line{ line }
		, _column{ column } {
	}

	auto Span::make_line(File const &file) const -> Span {
		auto offset = _offset;
		while (offset > 0 && file.content()[offset] != '\n') {
			--offset;
		}
		if (offset != 0) {
			++offset;
		}

		auto end = this->end();
		while (end < file.content().size() && file.content()[end] != '\n') {
			++end;
		}
		return Span{ offset, end - offset, _line, 1 };
	}

	auto Span::size() const -> usz {
		return _size;
	}

	auto Span::line() const -> usz {
		return _line;
	}

	auto Span::column() const -> usz {
		return _column;
	}

	auto Span::string(std::string_view const string) const -> std::string_view {
		return string.substr(_offset, _size);
	}

	auto Span::string(File const &file) const -> std::string_view {
		return string(file.content());
	}

	auto Span::to_string() const -> std::string {
		return std::to_string(_line) + ":" + std::to_string(_column);
	}

	auto Span::set_size(usz const size) -> Span & {
		_size = size;
		return *this;
	}

	auto Span::advance() -> void {
		++_offset;
		++_column;
	}

	auto Span::advance_newline() -> void {
		++_offset;
		++_line;
		_column = 0;
	}

	auto Span::end() const -> usz {
		return _offset + _size;
	}
}	 // iris