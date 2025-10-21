#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine {

	struct Text {
		entt::resource<FontResource> font;
		std::string text;
	};

	Text createText(entt::resource<sdl_engine::FontResource>& resource_, json& data_);
}