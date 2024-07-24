//
// Created by ruarq on 19/07/2024.
//

#ifndef FNTYPE_HPP
#define FNTYPE_HPP

#include "Type.hpp"

namespace iris::types {
	class FnType : public Type {
	public:
		using Param = std::pair<std::string, std::unique_ptr<Type>>;

	public:
		FnType(std::vector<Param> params, std::unique_ptr<Type> type);

	public:
		[[nodiscard]] auto is_fn_type() const -> bool override;
		[[nodiscard]] auto to_string() const -> std::string override;
		[[nodiscard]] auto equals(Type const *other) const -> bool override;
		[[nodiscard]] auto clone() const -> std::unique_ptr<Type> override;
		auto params() -> std::vector<Param> &;
		auto type() -> std::unique_ptr<Type> &;

		[[nodiscard]] auto width() const -> usz override;

		[[nodiscard]] auto is_read_only() const -> bool override;
		auto set_read_only(bool read_only) -> void override;

	private:
		std::vector<Param> _params;
		std::unique_ptr<Type> _type;
		bool _read_only = false;
	};

	template<typename... TTypes>
	auto make_params(TTypes &&...types) -> std::vector<FnType::Param> {
		std::vector<FnType::Param> result;
		(result.push_back(std::move(types)), ...);
		return result;
	}

	auto make_fn_type(std::vector<FnType::Param> params, std::unique_ptr<Type> type)
		-> std::unique_ptr<FnType>;
}

#endif	  //FNTYPE_HPP
