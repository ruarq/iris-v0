//
// Created by ruarq on 19/07/2024.
//

#ifndef TYPE_HPP
#define TYPE_HPP

#include "common.hpp"

namespace iris::types {
	enum class SimpleTypeKind;

	class Type {
	public:
		virtual ~Type() = default;

	public:
		[[nodiscard]] virtual auto is_fn_type() const -> bool;
		[[nodiscard]] virtual auto is_simple_type() const -> bool;
		[[nodiscard]] virtual auto is_simple_type(SimpleTypeKind) const -> bool;
		[[nodiscard]] virtual auto is_unit_type() const -> bool;
		[[nodiscard]] virtual auto is_integral() const -> bool;
		[[nodiscard]] virtual auto is_signed() const -> bool;
		[[nodiscard]] virtual auto is_unsigned() const -> bool;

		[[nodiscard]] virtual auto to_string() const -> std::string = 0;
		[[nodiscard]] virtual auto equals(Type const *other) const -> bool = 0;
		[[nodiscard]] virtual auto clone() const -> std::unique_ptr<Type> = 0;

		[[nodiscard]] virtual auto width() const -> usz = 0;

		[[nodiscard]] virtual auto is_read_only() const -> bool = 0;
		virtual auto set_read_only(bool read_only) -> void = 0;
	};

	auto operator==(Type const &lhs, Type const &rhs) -> bool;
	auto operator!=(Type const &lhs, Type const &rhs) -> bool;
}

#endif	  //TYPE_HPP
