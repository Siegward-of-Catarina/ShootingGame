#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/color.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <entt/entt.hpp>
struct SDL_Texture;
namespace sdl_engine
{
   struct Sprite
   {
      entt::resource<SpriteResource> texture;
      SDL_FRect                      src {};
      SDL_FRect                      dst {};
      ColorRGBA                      color;
   };
}    // namespace sdl_engine