//
// Created by ruarq on 19/07/2024.
//

#ifndef ERRORTYPE_HPP
#define ERRORTYPE_HPP

#include "Type.hpp"

namespace iris::types {
	class ErrorType : public Type {
	public:
		[[nodiscard]] auto to_string() const -> std::string override;
		[[nodiscard]] auto equals(Type const *) const -> bool override;
		[[nodiscard]] auto clone() const -> std::unique_ptr<Type> override;

		[[nodiscard]] auto is_read_only() const -> bool override;
		auto set_read_only(bool read_only) -> void override;

		[[nodiscard]] auto width() const -> usz override;

	private:
		bool _read_only = false;
	};

	[[nodiscard]] auto make_error_type() -> std::unique_ptr<ErrorType>;
}

#endif	  //ERRORTYPE_HPP
