#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/color.hpp>
namespace sdl_engine {
	struct FontResource {
		u32 width{};
		u32 height{};
		// フォント画像から切り出した矩形を保持
		// 上のwidth,heightをもとに切り出す
		std::vector<SDL_FRect> font_rect{};
	};
}

