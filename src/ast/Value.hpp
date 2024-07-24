//
// Created by ruarq on 18/07/2024.
//

#ifndef VALUE_HPP
#define VALUE_HPP

#include "common.hpp"

namespace iris::ast {
	class Value {
	public:
		enum class Kind {
			Int,
			Bool,
		};

		union Storage {
			i64 num_int;
			bool boolean;
		};

	public:
		explicit Value(i64 value);
		explicit Value(bool value);

	public:
		[[nodiscard]] auto to_string() const -> std::string;
		[[nodiscard]] auto kind() const -> Kind;

		template<typename T>
		[[nodiscard]] auto get() -> T & {
			// if (type_to_kind<T>() != _kind) {
			//	throw std::runtime_error{ "Invalid type for this value." };
			// }
			return *reinterpret_cast<T *>(&_storage);
		}

	private:
		Kind _kind;
		Storage _storage;
	};
}

#endif	  //VALUE_HPP
