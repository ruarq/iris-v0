//
// Created by ruarq on 22/07/2024.
//

#ifndef UNITTYPE_HPP
#define UNITTYPE_HPP

#include "Type.hpp"

namespace iris::types {
	class UnitType : public Type {
	public:
		[[nodiscard]] auto is_unit_type() const -> bool override;

		[[nodiscard]] auto to_string() const -> std::string override;
		[[nodiscard]] auto equals(Type const *other) const -> bool override;
		[[nodiscard]] auto clone() const -> std::unique_ptr<Type> override;

		[[nodiscard]] auto width() const -> usz override;

		[[nodiscard]] auto is_read_only() const -> bool override;
		auto set_read_only(bool) -> void override;
	};

	[[nodiscard]] auto make_unit_type() -> std::unique_ptr<UnitType>;
}

#endif	  //UNITTYPE_HPP
