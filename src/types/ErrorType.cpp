//
// Created by ruarq on 19/07/2024.
//

#include "ErrorType.hpp"

namespace iris::types {

	auto ErrorType::to_string() const -> std::string {
		return "<ErrorType>";
	}

	auto ErrorType::equals(Type const *) const -> bool {
		return false;
	}

	auto ErrorType::clone() const -> std::unique_ptr<Type> {
		return std::make_unique<ErrorType>(*this);
	}

	auto ErrorType::is_read_only() const -> bool {
		return _read_only;
	}

	auto ErrorType::set_read_only(bool const read_only) -> void {
		_read_only = read_only;
	}

	auto ErrorType::width() const -> usz {
		return 0;
	}

	auto make_error_type() -> std::unique_ptr<ErrorType> {
		return std::make_unique<ErrorType>();
	}
}