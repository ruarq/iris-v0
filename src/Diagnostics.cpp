//
// Created by ruarq on 18/07/2024.
//

#include "Diagnostics.hpp"

#include <nlohmann/json.hpp>

namespace iris {
	Diagnostics::Diagnostics(File const &file)
		: _file{ file } {
	}

	auto Diagnostics::add_template(std::string const &name, MessageTemplate message_template)
		-> void {
		_templates[name] = std::move(message_template);
	}

	auto Diagnostics::had_messages() const -> bool {
		return had_errors() || had_warnings();
	}

	auto Diagnostics::had_errors() const -> bool {
		return _error_count > 0;
	}

	auto Diagnostics::had_warnings() const -> bool {
		return _warning_count > 0;
	}

	auto Diagnostics::get_report() const -> std::string {
		std::string report;
		for (auto const &message : _messages) {
			report += message.format(_file) + "\n";
		}
		return report.substr(0, report.size() - 1);
	}

	auto parse_message_templates(nlohmann::json const &json)
		-> std::vector<std::pair<std::string, MessageTemplate>> {
		std::vector<std::pair<std::string, MessageTemplate>> result;
		for (auto const &message_template_json : json) {
			auto message_template = message_template_json.get<MessageTemplate>();
			auto name = message_template_json.at("name");
			result.emplace_back(name, message_template);
		}

		return result;
	}
}