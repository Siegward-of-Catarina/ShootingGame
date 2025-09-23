#pragma once
#include <SDL3/SDL_rect.h>
#include <core/fwd.hpp>
#include <vector>
struct SDL_Texture;
namespace myge
{
   struct SpriteAnimComponent
   {
      std::vector<SDL_FRect> frames;
      u32                    frame_width;
      u32                    frame_height;
      u32                    frame_num;
      f64                    frame_time;    // •b’PˆÊ
      bool                   loop;

      u32  current_frame;
      f64  elapsedTime;    // •b’PˆÊ
      bool is_playing;
   };
}    // namespace myge