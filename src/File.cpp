//
// Created by ruarq on 17/07/2024.
//

#include "File.hpp"

#include <fstream>

namespace iris {
	auto File::open(std::filesystem::path const &path) -> File {
		std::ifstream file{ path };
		if (!file.is_open()) {
			throw std::runtime_error{ "Couldn't open '" + absolute(path).string() + "'" };
		}

		std::string const content{ std::istreambuf_iterator{ file },
			std::istreambuf_iterator<char>{} };
		return File{ path, content };
	}

	File::File(std::filesystem::path path, std::string content) noexcept
		: _path{ std::move(path) }
		, _content{ std::move(content) } {
	}

	auto File::path() const noexcept -> std::filesystem::path {
		return _path;
	}

	auto File::content() const noexcept -> std::string_view {
		return _content;
	}
}	 // iris