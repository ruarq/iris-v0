//
// Created by ruarq on 17/07/2024.
//

#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem>

#include "common.hpp"

namespace iris {
	class File {
	public:
		[[nodiscard]] static auto open(std::filesystem::path const &path) -> File;

	public:
		File() = default;
		File(std::filesystem::path path, std::string content) noexcept;

	public:
		[[nodiscard]] auto path() const noexcept -> std::filesystem::path;
		[[nodiscard]] auto content() const noexcept -> std::string_view;

	private:
		std::filesystem::path _path;
		std::string _content;
	};
}	 // iris

#endif	  //FILE_HPP
