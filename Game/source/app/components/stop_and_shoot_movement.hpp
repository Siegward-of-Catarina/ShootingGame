#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
namespace myge
{
   struct StopAndShootMovement
   {
      f32                     speed    = {};
      sdl_engine::Vector2_f32 stop_pos = {};
      u32                     num_shots {};
      f32                     shoot_interval = {};
      enum class State
      {
         Entering,
         Shooting,
         Exiting
      } state = State::Entering;
   };
}    // namespace myge