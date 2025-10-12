#pragma once
#include <engine/math.hpp>
namespace myge
{
   struct PlayerInput
   {
      sdl_engine::Vector2_f32 move_direction;
      bool                    isShoot;
   };

   PlayerInput createPlayerInput();
}    // namespace myge