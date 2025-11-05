#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
namespace myge
{
   struct PlayerMovementInput
   {
      f32                     max_speed {};
      f32                     acceleration {};
      f32                     deceleration {};
      sdl_engine::Vector2_f32 smooth_dir {};
   };
   struct PlayerMovementAuto
   {
      f32                     speed {};
      sdl_engine::Vector2_f32 direction {};
   };
}    // namespace myge