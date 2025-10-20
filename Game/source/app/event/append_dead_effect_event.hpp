#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct AppedDeadEffectEvent
   {
      std::vector<entt::entity> dead_entities;
   };
}    // namespace myge