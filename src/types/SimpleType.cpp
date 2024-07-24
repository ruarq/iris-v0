//
// Created by ruarq on 19/07/2024.
//

#include "SimpleType.hpp"

namespace iris::types {
	SimpleType::SimpleType(Kind const kind)
		: _kind{ kind }
		, _read_only{ true } {
	}

	auto SimpleType::is_simple_type() const -> bool {
		return true;
	}

	auto SimpleType::is_simple_type(SimpleTypeKind const kind) const -> bool {
		return kind == _kind;
	}

	auto SimpleType::is_integral() const -> bool {
		return is_unsigned() || is_signed();
	}

	auto SimpleType::is_signed() const -> bool {
		using enum Kind;
		switch (_kind) {
			case Int8:
			case Int16:
			case Int32:
			case Int64:
			case ISize: return true;
			default: break;
		}
		return false;
	}

	auto SimpleType::is_unsigned() const -> bool {
		using enum Kind;
		switch (_kind) {
			case UInt8:
			case UInt16:
			case UInt32:
			case UInt64:
			case USize: return true;
			default: break;
		}
		return false;
	}

	auto SimpleType::to_string() const -> std::string {
		return std::string{ types::to_string(_kind) };
	}

	auto SimpleType::equals(Type const *other) const -> bool {
		if (auto const simple_type = dynamic_cast<SimpleType const *>(other)) {
			return _kind == simple_type->_kind;
		}
		return false;
	}

	auto SimpleType::clone() const -> std::unique_ptr<Type> {
		return std::make_unique<SimpleType>(*this);
	}

	auto SimpleType::width() const -> usz {
		using enum Kind;
		switch (_kind) {
			case Bool:
			case Int8:
			case UInt8: return 1;

			case Int16:
			case UInt16: return 2;

			case Int32:
			case UInt32: return 4;

			case Int64:
			case UInt64: return 8;

			// Well, it actually depends on the platform we're compiling to but whatever...
			// TODO(ruarq)...
			case USize:
			case ISize: return sizeof(size_t);
		}
	}

	auto SimpleType::is_read_only() const -> bool {
		return _read_only;
	}

	auto SimpleType::set_read_only(bool const read_only) -> void {
		_read_only = read_only;
	}

	auto make_bool_type() -> std::unique_ptr<SimpleType> {
		return make_simple_type(SimpleType::Kind::Bool);
	}

	auto make_i32_type() -> std::unique_ptr<SimpleType> {
		return make_simple_type(SimpleType::Kind::Int32);
	}

	auto make_simple_type(SimpleType::Kind const kind) -> std::unique_ptr<SimpleType> {
		return std::make_unique<SimpleType>(kind);
	}

	auto to_string(SimpleType::Kind const kind) -> std::string_view {
		using enum SimpleType::Kind;
		switch (kind) {
			case Int8: return "i8";
			case Int16: return "i16";
			case Int32: return "i32";
			case Int64: return "i64";

			case UInt8: return "u8";
			case UInt16: return "u16";
			case UInt32: return "u32";
			case UInt64: return "u64";

			case ISize: return "isz";
			case USize: return "usz";

			case Bool: return "bool";
		}

		throw std::runtime_error{ "Invalid value for SimpleType::Kind" };
	}
}