#pragma once
#include <SDL3/SDL_rect.h>
struct SDL_Texture;
namespace myge {
	struct SpriteComponent {
		SDL_Texture* texture;
		SDL_FRect rect;

	};
}