#pragma once
#include <SDL3/SDL_rect.h>
#include <core/fwd.hpp>
#include <vector>
namespace myge
{
   struct SpriteAnimResource
   {
      std::string            sprite_name;
      std::vector<SDL_FRect> frames;
      u32                    frame_width{};
      u32                    frame_height{};
      u32                    frame_num{};
      f64                    frame_time{};    // •b’PˆÊ
      bool                   is_loop{};
   };

}    // namespace myge
