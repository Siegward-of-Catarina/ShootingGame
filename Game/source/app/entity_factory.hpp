#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
#include <engine/utils.hpp>
#include <typeindex>
namespace sdl_engine {
    struct Transform;
}
namespace myge
{
   class EntityFactory
   {
   public:
      EntityFactory( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_ );
      ~EntityFactory();

      entt::entity createDefaultFadeEntity( f32 window_width, f32 window_height );
      entt::entity createBullet( entt::entity& shooter_, const std::type_index& affiliation_id_ );
      entt::entity createHitEffect(sdl_engine::Transform& bullet_trfm_, const std::type_index& affiliation_id_);
      entt::entity createPlayer( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createWandererEnemy( json&                   data_,
                                        const std::type_index&  affiliation_id_,
                                        sdl_engine::Vector2_f32 offset_pos_ = { 0.0f, 0.0f } );
      entt::entity createBasicUI( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createHighlightableUI( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createBasicText( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createBrinkText( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createHighlightableText( json& data_, const std::type_index& affiliation_id_ );
      entt::entity createTitleMenu( json& data_, const std::type_index& affiliation_id_ );
      std::pair<entt::entity, entt::entity> createBackGround( json& data_, const std::type_index& affiliation_id_ );
      std::vector<entt::entity> createWandererEnemyArray( json& data_, const std::type_index& affiliation_id_ );
      std::vector<entt::entity> createEntities( json&                           data_,
                                                const std::type_index&          affiliation_id_,
                                                const std::vector<std::string>& exclude = {} );

   private:
      void setAffiliationTag( entt::entity entity_, const std::type_index& affiliation_id_ );

   private:
      entt::registry&              _registry;
      sdl_engine::ResourceManager& _resource_manager;
   };
}    // namespace myge
