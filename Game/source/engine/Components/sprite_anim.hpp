#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
#include <vector>
struct SDL_Texture;
namespace sdl_engine
{
   struct SpriteAnim
   {
      entt::resource<SpriteAnimResource> sprite_anim;
      f32                                current_frame {};
      f64                                elapsed_time {};    // •b’PˆÊ
      bool                               is_playing {};
   };
}    // namespace sdl_engine