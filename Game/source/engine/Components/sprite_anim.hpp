#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/core/forward_declarations.hpp>
#include <vector>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
	struct SpriteAnim
	{
		entt::resource<SpriteAnimResource> sprite_anim;
		u32                                current_frame;
		f64                                elapsed_time;    // •b’PˆÊ
		bool                               is_playing;
	};
	SpriteAnim createSpriteAnim(entt::resource<sdl_engine::SpriteAnimResource> resource_, const json& data_);
}    // namespace sdl_engine