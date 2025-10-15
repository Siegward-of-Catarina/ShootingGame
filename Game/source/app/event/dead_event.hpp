#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct DeadEvent
   {
      std::unordered_set<entt::entity> dead_entities;
   };
}    // namespace myge