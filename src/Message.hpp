//
// Created by ruarq on 18/07/2024.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Span.hpp"
#include "common.hpp"

namespace iris {
	class Message {
	public:
		enum class Kind {
			Error,
			Warning,
		};

	public:
		Message(Span const &span, usz id, Kind kind, std::string message);

	public:
		[[nodiscard]] auto format(File const &file) const -> std::string;

		[[nodiscard]] auto kind() const -> Kind;

	private:
		Span _span;
		usz _id;
		Kind _kind;
		std::string _message;
	};

	auto to_string(Message::Kind kind) -> std::string_view;
	auto from_string(std::string_view string) -> Message::Kind;
}

#endif	  //MESSAGE_HPP
