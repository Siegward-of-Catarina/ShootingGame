#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/rendering/loader/sprite_resource.hpp>
#include <entt/entt.hpp>
struct SDL_Texture;
namespace sdl_engine
{
   struct Sprite
   {
      entt::resource<SpriteResource> texture;
      SDL_FRect                      src {};
      SDL_FRect                      dst {};
   };
}    // namespace sdl_engine