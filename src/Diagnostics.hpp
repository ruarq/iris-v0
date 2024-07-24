//
// Created by ruarq on 18/07/2024.
//

#ifndef DIAGNOSTICS_HPP
#define DIAGNOSTICS_HPP

#include <unordered_map>

#include "File.hpp"
#include "Message.hpp"
#include "MessageTemplate.hpp"

namespace iris {
	class Diagnostics {
	public:
		explicit Diagnostics(File const &file);

	public:
		auto add_template(std::string const &name, MessageTemplate message_template) -> void;

		template<typename... TFormatArgs>
		auto emit(Span const &span, std::string const &message, TFormatArgs &&...format_args)
			-> void {
			if (!_templates.contains(message)) {
				throw std::runtime_error{ "No MessageTemplate for '" + std::string{ message } +
										  "'" };
			}

			auto const result =
				_templates.at(message).build(span, std::forward<TFormatArgs>(format_args)...);
			if (result.kind() == Message::Kind::Error) {
				++_error_count;
			} else if (result.kind() == Message::Kind::Warning) {
				++_warning_count;
			}

			_messages.push_back(result);
		}

		[[nodiscard]] auto had_messages() const -> bool;
		[[nodiscard]] auto had_errors() const -> bool;
		[[nodiscard]] auto had_warnings() const -> bool;
		auto get_report() const -> std::string;

	private:
		File const &_file;
		std::vector<Message> _messages;
		std::unordered_map<std::string, MessageTemplate> _templates;

		usz _error_count = 0;
		usz _warning_count = 0;
	};

	auto parse_message_templates(nlohmann::json const &json)
		-> std::vector<std::pair<std::string, MessageTemplate>>;
}

#endif	  //DIAGNOSTICS_HPP
