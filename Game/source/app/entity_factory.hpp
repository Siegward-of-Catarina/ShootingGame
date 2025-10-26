#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
#include <engine/utils.hpp>
#include <typeindex>
namespace sdl_engine
{
   struct Transform;
   struct Fade;
}    // namespace sdl_engine
namespace myge
{
   class EntityFactory
   {
   public:
      EntityFactory( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_ );
      ~EntityFactory();

      entt::entity createDefaultFadeEntity( const std::string_view type_,
                                            f32                    window_width_,
                                            f32                    window_height_,
                                            f32                    end_alpha_          = -1.0f,    // option
                                            f32                    target_out_alpha_   = -1.0f,    // option
                                            f32                    speed_              = -1.0f,    // option
                                            f32                    black_out_duration_ = -1.0f,    // option
                                            bool                   under_ui_           = false );

      entt::entity createSoundEffect( const std::string_view resource_key_, const i64 loop_count_, const f32 volume_ );
      entt::entity createBGM( const std::string_view resource_key_,
                              const i64              loop_count_,
                              const i64              fade_time_ms_,
                              const f32              volume_ );
      entt::entity createBullet( entt::entity& shooter_, const std::type_index& affiliation_id_ );
      entt::entity createHitEffect( entt::entity dead_entt_, const std::type_index& affiliation_id_ );
      entt::entity createPlayer( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createPlayerBooster( const entt::entity parent_, const std::type_index& affiliation_id_ );
      entt::entity createWandererEnemy( const json&             data_,
                                        const std::type_index&  affiliation_id_,
                                        sdl_engine::Vector2_f32 offset_pos_ = { 0.0f, 0.0f } );
      entt::entity createBasicUI( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createHighlightableUI( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createBasicText( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createBrinkText( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createHighlightableText( const json& data_, const std::type_index& affiliation_id_ );
      entt::entity createTitleMenu( const json& data_, const std::type_index& affiliation_id_ );
      std::pair<entt::entity, entt::entity> createBackGround( const json&            data_,
                                                              const std::type_index& affiliation_id_ );
      std::vector<entt::entity> createWandererEnemyArray( const json& data_, const std::type_index& affiliation_id_ );
      std::vector<entt::entity> createEntities( const json&                     data_,
                                                const std::type_index&          affiliation_id_,
                                                const std::vector<std::string>& exclude = {} );

   private:
      void setAffiliationTag( entt::entity entity_, const std::type_index& affiliation_id_ );

   private:
      entt::registry&              _registry;
      sdl_engine::ResourceManager& _resource_manager;
   };
}    // namespace myge
