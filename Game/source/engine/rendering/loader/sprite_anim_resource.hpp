#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/core/forward_declarations.hpp>
#include <vector>
namespace sdl_engine
{
   struct SpriteAnimResource
   {
      std::vector<SDL_FRect> frames;
      u32                    frame_width {};
      u32                    frame_height {};
      u32                    frame_num {};
      f64                    frame_time {};    // •b’PˆÊ
      bool                   is_loop {};
   };

}    // namespace sdl_engine
