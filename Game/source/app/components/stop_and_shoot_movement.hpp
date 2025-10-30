#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
namespace myge
{
   struct StopAndShootMovement
   {
      f32                     speed        = {};
      sdl_engine::Vector2_f32 stop_pos     = {};
      sdl_engine::Vector2_f32 exit_pos     = {};
      sdl_engine::Vector2_f32 pre_trfm_pos = {};
      enum class State
      {
         Entering,
         Shooting,
         Exiting
      } state = State::Entering;
   };
}    // namespace myge