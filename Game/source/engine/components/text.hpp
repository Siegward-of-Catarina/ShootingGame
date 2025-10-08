#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/color.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine {

	struct Text {
		std::string text;
		enum class Size {
			Small,	// 8px
			Medium,	// 16px
			Large	// 24px
		} size;
		ColorRGBA color;
	};

	Text createText(const json& data_);
}