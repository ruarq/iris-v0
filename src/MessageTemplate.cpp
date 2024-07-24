//
// Created by ruarq on 18/07/2024.
//

#include "MessageTemplate.hpp"

namespace iris {
	MessageTemplate::MessageTemplate(usz const id,
		Message::Kind const kind,
		std::string fmt,
		std::string description)
		: _id{ id }
		, _kind{ kind }
		, _fmt{ std::move(fmt) }
		, _description{ std::move(description) } {
	}

	auto from_json(nlohmann::json const &json, MessageTemplate &message_template) -> void {
		usz id;
		std::string kind;
		std::string fmt;
		std::string description;
		json.at("id").get_to(id);
		json.at("kind").get_to(kind);
		json.at("fmt").get_to(fmt);
		json.at("description").get_to(description);
		message_template = MessageTemplate{ id, from_string(kind), fmt, description };
	}
}