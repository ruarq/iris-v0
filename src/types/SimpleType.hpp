//
// Created by ruarq on 19/07/2024.
//

#ifndef SIMPLETYPE_HPP
#define SIMPLETYPE_HPP

#include "Type.hpp"

namespace iris::types {
	enum class SimpleTypeKind {
		Int8,
		Int16,
		Int32,
		Int64,

		UInt8,
		UInt16,
		UInt32,
		UInt64,

		ISize,
		USize,

		Bool,
	};

	class SimpleType : public Type {
	public:
		using Kind = SimpleTypeKind;

	public:
		explicit SimpleType(Kind kind);

	public:
		[[nodiscard]] auto is_simple_type() const -> bool override;
		[[nodiscard]] auto is_simple_type(SimpleTypeKind kind) const -> bool override;
		[[nodiscard]] auto is_integral() const -> bool override;
		[[nodiscard]] auto is_signed() const -> bool override;
		[[nodiscard]] auto is_unsigned() const -> bool override;

		[[nodiscard]] auto to_string() const -> std::string override;
		[[nodiscard]] auto equals(Type const *other) const -> bool override;
		[[nodiscard]] auto clone() const -> std::unique_ptr<Type> override;

		[[nodiscard]] auto width() const -> usz override;

		[[nodiscard]] auto is_read_only() const -> bool override;
		auto set_read_only(bool read_only) -> void override;

	private:
		Kind _kind;
		bool _read_only;
	};

	[[nodiscard]] auto make_bool_type() -> std::unique_ptr<SimpleType>;
	[[nodiscard]] auto make_i32_type() -> std::unique_ptr<SimpleType>;
	[[nodiscard]] auto make_simple_type(SimpleType::Kind kind) -> std::unique_ptr<SimpleType>;
	[[nodiscard]] auto to_string(SimpleType::Kind kind) -> std::string_view;
}

#endif	  //SIMPLETYPE_HPP
