#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class EntityFactory
   {
   public:
      void createEntities( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_, json& data_ );

   private:
      // ResourceManager&;
   };
}    // namespace myge
