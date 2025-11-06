#pragma once
#include <engine/components/transform.hpp>
namespace myge
{
   struct HitEvent
   {
      std::vector<std::pair<entt::entity, entt::entity>> hit_entity_pairs;
   };
}    // namespace myge