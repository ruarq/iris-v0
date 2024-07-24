//
// Created by ruarq on 18/07/2024.
//

#ifndef MESSAGETEMPLATE_HPP
#define MESSAGETEMPLATE_HPP

#include <nlohmann/json.hpp>

#include "Message.hpp"
#include "Span.hpp"
#include "common.hpp"

namespace iris {
	class MessageTemplate {
	public:
		MessageTemplate() = default;
		MessageTemplate(usz id, Message::Kind kind, std::string fmt, std::string description);

	public:
		template<typename... TFormatArgs>
		[[nodiscard]] auto build(Span const span, TFormatArgs &&...format_args) -> Message {
			try {
				auto const message =
					fmt::format(fmt::runtime(_fmt), std::forward<TFormatArgs>(format_args)...);
				return Message{ span, _id, _kind, message };
			} catch (std::exception &e) {
				throw std::runtime_error{ fmt::format(
					"MessageTemplate::build() : _fmt = '{}', e.what() = '{}'\n",
					_fmt,
					e.what()) };
			}
		}

	private:
		usz _id;
		Message::Kind _kind;
		std::string _fmt;
		std::string _description;
	};

	auto from_json(nlohmann::json const &json, MessageTemplate &message_template) -> void;
}

#endif	  //MESSAGETEMPLATE_HPP
