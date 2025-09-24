#pragma once
#include <entt/entt.hpp>
#include <SDL3/SDL_rect.h>
struct SDL_Texture;
namespace myge {
	struct SpriteComponent {
		entt::resource<SDL_Texture> sprite;
		SDL_FRect src{};
		SDL_FRect dst{};
		u16 pz{}; //•`‰æ‡‚ğŒˆ’è‚·‚é
	};
}