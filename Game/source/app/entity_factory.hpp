#pragma once
#include <engine/forward.hpp>
#include <engine/utils.hpp>
namespace myge
{

   class EntityFactory
   {
   public:
      entt::entity
      createBullet( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_, entt::entity& shooter_ );
      std::vector<entt::entity>
      createEntities( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_, json& data_ );
   };
}    // namespace myge
