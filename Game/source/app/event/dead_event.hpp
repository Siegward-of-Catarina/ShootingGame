#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct DeadEvent
   {
      std::vector<entt::entity> dead_entities;
      bool                      out_of_screen_death {};
   };
}    // namespace myge