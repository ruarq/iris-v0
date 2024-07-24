//
// Created by ruarq on 19/07/2024.
//

#include "Environment.hpp"
namespace iris {
	Environment::Environment(Environment *parent)
		: _parent{ parent } {
	}

	auto Environment::create_child() -> Environment {
		return Environment{ this };
	}

	auto Environment::insert(std::string const &key, std::unique_ptr<types::Type> type) -> void {
		_map[key] = std::move(type);
	}

	auto Environment::lookup(std::string const &key) -> std::unique_ptr<types::Type> * {
		if (!_map.contains(key)) {
			if (_parent) {
				return _parent->lookup(key);
			}

			return nullptr;
		}

		return &_map.at(key);
	}

}