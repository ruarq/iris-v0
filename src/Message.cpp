//
// Created by ruarq on 18/07/2024.
//

#include "Message.hpp"

#include <unordered_map>

namespace iris {
	Message::Message(Span const &span, usz const id, Kind const kind, std::string message)
		: _span{ span }
		, _id{ id }
		, _kind{ kind }
		, _message{ std::move(message) } {
	}

	auto Message::format(File const &file) const -> std::string {
		auto const header = fmt::format("|- {}({:0>4X}): {}\n", to_string(_kind), _id, _message);
		auto line = _span.make_line(file).string(file);
		auto const body = fmt::format("| {:>4} {}", _span.line(), line);
		std::string pointer;
		for (usz i = 0; i < _span.column() - 1; ++i) {
			switch (line[i]) {
				case '\t': pointer += "\t"; break;
				default: pointer += " "; break;
			}
		}
		for (usz i = 0; i < _span.size(); ++i) {
			pointer += "^";
		}
		return header + body + fmt::format("\n|      {}", pointer);
	}

	auto Message::kind() const -> Kind {
		return _kind;
	}

#define _(x) [static_cast<int>(x)] = #x

	auto to_string(Message::Kind kind) -> std::string_view {
		using enum Message::Kind;
		constexpr std::string_view table[] = { _(Error), _(Warning) };
		return table[static_cast<int>(kind)];
	}

#undef _

	auto from_string(std::string_view const string) -> Message::Kind {
		using enum Message::Kind;
		if (std::unordered_map<std::string_view, Message::Kind> const map{
				{ "Error",   Error	 },
				{ "Warning", Warning },
		};
			map.contains(string)) {
			return map.at(string);
		}

		throw std::runtime_error{ "Message::Kind::" + std::string{ string } + " doesn't exist" };
	}

}