//
// Created by ruarq on 19/07/2024.
//

#include "FnType.hpp"

namespace iris::types {

	FnType::FnType(std::vector<Param> params, std::unique_ptr<Type> type)
		: _params{ std::move(params) }
		, _type{ std::move(type) } {
	}

	bool FnType::is_fn_type() const {
		return true;
	}

	auto FnType::to_string() const -> std::string {
		std::vector<std::string> params;
		for (auto const &[id, type] : _params) {
			params.push_back(fmt::format("{}: {}", id, type->to_string()));
		}

		return fmt::format("fn({}): {}", params, _type->to_string());
	}

	auto FnType::equals(Type const *other) const -> bool {
		if (auto const fn_type = dynamic_cast<FnType const *>(other)) {
			if (fn_type->_params.size() != _params.size()) {
				return false;
			}

			for (usz i = 0; i < _params.size(); ++i) {
				auto const &[id, type] = _params[i];
				auto const &[other_id, other_type] = fn_type->_params[i];

				if (id != other_id && *type != *other_type) {
					return false;
				}
			}

			return *fn_type->_type == *_type;
		}

		return false;
	}

	auto FnType::clone() const -> std::unique_ptr<Type> {
		std::vector<Param> params;
		for (auto const &[id, type] : _params) {
			params.emplace_back(id, type->clone());
		}
		auto type = std::make_unique<FnType>(std::move(params), _type->clone());
		type->set_read_only(_read_only);
		return type;
	}

	auto FnType::params() -> std::vector<Param> & {
		return _params;
	}

	auto FnType::type() -> std::unique_ptr<Type> & {
		return _type;
	}

	auto FnType::width() const -> usz {
		return sizeof(void *);	  // Also depends on target platform... TODO(ruarq)
	}

	auto FnType::is_read_only() const -> bool {
		return _read_only;
	}

	auto FnType::set_read_only(bool const read_only) -> void {
		_read_only = read_only;
	}

	auto make_fn_type(std::vector<FnType::Param> params, std::unique_ptr<Type> type)
		-> std::unique_ptr<FnType> {
		return std::make_unique<FnType>(std::move(params), std::move(type));
	}
}