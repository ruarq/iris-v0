//
// Created by ruarq on 22/07/2024.
//

#include "UnitType.hpp"

namespace iris::types {
	auto UnitType::is_unit_type() const -> bool {
		return true;
	}

	auto UnitType::to_string() const -> std::string {
		return "()";
	}

	auto UnitType::equals(Type const *other) const -> bool {
		return other->is_unit_type();
	}

	auto UnitType::clone() const -> std::unique_ptr<Type> {
		return std::make_unique<UnitType>();
	}

	auto UnitType::width() const -> usz {
		return 0;
	}

	auto UnitType::is_read_only() const -> bool {
		return true;
	}

	auto UnitType::set_read_only(bool) -> void {
	}

	auto make_unit_type() -> std::unique_ptr<UnitType> {
		return std::make_unique<UnitType>();
	}

}