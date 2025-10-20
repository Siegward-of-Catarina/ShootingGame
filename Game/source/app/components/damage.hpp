#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct DamageEffect
   {
      f32 red_brink_time {};
      f32 brink_interval {};    // time / count
      f32 elapsed_time { 0.f };
   };
}    // namespace myge