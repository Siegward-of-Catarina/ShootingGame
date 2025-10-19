#pragma once
#include <engine/forward.hpp>
#include <entt/entt.hpp>
namespace myge
{
   struct EnemyFollower
   {
      entt::entity leader {};
      u32          delay_frames {};
   };
}    // namespace myge