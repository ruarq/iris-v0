//
// Created by ruarq on 18/07/2024.
//

#include "Value.hpp"

namespace iris::ast {
	Value::Value(i64 const value)
		: _kind{ Kind::Int }
		, _storage{ .num_int = value } {
	}

	Value::Value(bool const value)
		: _kind{ Kind::Bool }
		, _storage{ .num_int = value } {
	}

	auto Value::to_string() const -> std::string {
		switch (_kind) {
			case Kind::Int: return fmt::format("{}", _storage.num_int);
			case Kind::Bool: return fmt::format("{}", _storage.boolean);
		}

		throw std::runtime_error{ "Invalid Value::kind()" };
	}

	auto Value::kind() const -> Kind {
		return _kind;
	}
}