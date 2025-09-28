#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class EntityFactory
   {
   public:
      entt::entity
      createEnemy( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_, const json& data_ );
      entt::entity
      createBackGround( entt::registry& registry_, sdl_engine::ResourceManager& resource_mamager_, const json& data_ );

   private:
      // ResourceManager&;
   };
}    // namespace myge
