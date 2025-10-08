#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/color.hpp>
namespace sdl_engine {
	struct FontResource {
		u32 width{};
		u32 height{};
		// �t�H���g�摜����؂�o������`��ێ�
		// ���width,height�����Ƃɐ؂�o��
		std::vector<SDL_FRect> font_rect{};
	};
}

