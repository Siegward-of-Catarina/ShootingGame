#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
namespace myge
{
   struct SinWaveMovement
   {
      f32                     amplitude {};
      f32                     frequency {};
      f32                     move_speed {};
      sdl_engine::Vector2_f32 direction {};
      f32                     time {};
   };
}    // namespace myge