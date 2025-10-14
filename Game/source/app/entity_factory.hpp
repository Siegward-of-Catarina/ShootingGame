#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
#include <engine/utils.hpp>
namespace myge
{

   class EntityFactory
   {
   public:
      EntityFactory( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_ );
      ~EntityFactory();

      entt::entity              createDefaultFadeEntity( f32 window_width, f32 window_height );
      entt::entity              createBullet( entt::entity& shooter_ );
      std::vector<entt::entity> createEntities( json& data_ );
      std::vector<entt::entity> createWandererEnemyArray( json& data_ );
      entt::entity createWandererEnemy( json& data_, sdl_engine::Vector2_f32 offset_pos_ = { 0.0f, 0.0f } );

   private:
      entt::registry&              _registry;
      sdl_engine::ResourceManager& _resource_manager;
   };
}    // namespace myge
