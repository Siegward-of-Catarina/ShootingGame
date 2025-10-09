#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine {

	class FontLoader final
	{
	public:
		using result_type = std::shared_ptr<FontResource>;
		result_type operator()(const json& data)const;
	};
}

