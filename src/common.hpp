//
// Created by ruarq on 17/07/2024.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#define IRIS_NOT_IMPLEMENTED(x) (throw std::runtime_error{ #x " : Not implemented" })

namespace iris {
	using usz = unsigned long;

	using i64 = long long;
}

#endif	  //COMMON_HPP
