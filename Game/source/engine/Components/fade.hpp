#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   struct Fade
   {
      enum class State
      {
         FadeIn,
         FadeOut,
         BlackOut,
         FadeInEnd,
         FadeOutEnd,
         Idel,
      } state{State::Idel};
      f32 speed {};
      f32 black_out_time {};
   };
}    // namespace sdl_engine