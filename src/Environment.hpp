//
// Created by ruarq on 19/07/2024.
//

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>

#include "types/types.hpp"

namespace iris {
	class Environment {
	public:
		Environment() = default;
		explicit Environment(Environment *parent);

	public:
		auto create_child() -> Environment;
		auto insert(std::string const &key, std::unique_ptr<types::Type> type) -> void;
		auto lookup(std::string const &key) -> std::unique_ptr<types::Type> *;

	private:
		Environment *_parent = nullptr;
		std::unordered_map<std::string, std::unique_ptr<types::Type>> _map;
	};
}

#endif	  //ENVIRONMENT_HPP
