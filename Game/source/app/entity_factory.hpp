#pragma once
#include <engine/forward.hpp>
#include <engine/utils.hpp>
namespace myge
{
   class EntityFactory
   {
   public:
      std::vector<entt::entity> createEntities( sdl_engine::GameContext& context_, json& data_ );
   };
}    // namespace myge
