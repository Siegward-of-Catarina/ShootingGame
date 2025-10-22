#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct AppendDeadEffectEvent
   {
      std::vector<entt::entity> dead_entities;
   };
}    // namespace myge