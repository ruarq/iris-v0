//
// Created by ruarq on 19/07/2024.
//

#include "Type.hpp"

namespace iris::types {
	auto Type::is_fn_type() const -> bool {
		return false;
	}

	auto Type::is_simple_type() const -> bool {
		return false;
	}

	auto Type::is_simple_type(SimpleTypeKind) const -> bool {
		return false;
	}

	auto Type::is_unit_type() const -> bool {
		return false;
	}

	auto Type::is_integral() const -> bool {
		return false;
	}

	auto Type::is_signed() const -> bool {
		return false;
	}

	auto Type::is_unsigned() const -> bool {
		return false;
	}

	auto operator==(Type const &lhs, Type const &rhs) -> bool {
		return lhs.equals(&rhs);
	}

	auto operator!=(Type const &lhs, Type const &rhs) -> bool {
		return !(lhs == rhs);
	}

}