#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/core/game_exception.hpp>
#include <nlohmann/json.hpp>
#include <source_location>
#include <SDL3/SDL_log.h>
#include <optional>
using json = nlohmann::json;

namespace sdl_engine
{
	json loadJson(std::string_view assets_path_);
	template<typename T>
	std::optional<T> getJsonData(const json& data_, std::string_view key_, const std::source_location location = std::source_location::current())
	{
		if (!data_.contains(key_.data()))
		{
			std::string error_msg =
				"éwíËÇ≥ÇÍÇΩÉLÅ[Ç™ë∂ç›ÇµÇ‹ÇπÇÒ: "
				+ std::string(key_)
				+ ", file :"
				+ location.file_name();
			SDL_Log(error_msg.c_str());
			return std::nullopt;
		}
		return data_.at(key_.data()).get<T>();
	}
}    // namespace sdl_engine