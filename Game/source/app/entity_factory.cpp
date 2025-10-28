// my header
#include <app/entity_factory.hpp>

// components
#include <app/components/affiliation.hpp>
#include <app/components/bounding_box.hpp>
#include <app/components/button_ui.hpp>
#include <app/components/damage_effect_property.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/highlightable.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/player_movement.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/components/shooter.hpp>
#include <app/components/sin_wave_movement.hpp>
#include <app/components/sprite_brink.hpp>
#include <app/components/status.hpp>
#include <app/components/title_input.hpp>
#include <app/components/title_menu.hpp>
#include <app/components/transform_link.hpp>

// basic_components
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/utils.hpp>

namespace myge
{
   EntityFactory::EntityFactory( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_ )
     : _registry { registry_ }, _resource_manager { resource_manager_ }
   {
   }
   EntityFactory::~EntityFactory() {}

   // ライフタイム管理は外部で行うこと
   entt::entity EntityFactory::createDefaultFadeEntity( const std::string_view type_,
                                                        f32                    window_width_,
                                                        f32                    window_height_,
                                                        f32                    end_alpha_,             // option
                                                        f32                    target_out_alpha_,      // option
                                                        f32                    speed_,                 // option
                                                        f32                    black_out_duration_,    // option
                                                        bool                   under_ui_ )
   {
      auto fade { _registry.create() };
      // transfrom
      sdl_engine::Transform trfm_cmp {
         .position { window_width_ / 2.0f, window_height_ / 2.0f },
          .angle { 0.0f },
          .scale { 1.0f }
      };
      _registry.emplace<sdl_engine::Transform>( fade, trfm_cmp );
      // sprite
      sdl_engine::Sprite sprt_comp {};
      sprt_comp.texture = _resource_manager.getSprite( "white" );
      sprt_comp.src     = { 0.0f, 0.0f, window_width_, window_height_ };
      sprt_comp.dst     = { trfm_cmp.position.x, trfm_cmp.position.y, window_width_, window_height_ };
      sprt_comp.color   = { 0.0f, 0.0f, 0.0f, 0.0f };
      _registry.emplace<sdl_engine::Sprite>( fade, sprt_comp );
      // fade
      sdl_engine::Fade fade_comp { .state { sdl_engine::Fade::State::Idle },
                                   .type {},
                                   .end_alpha { ( end_alpha_ >= 0 ) ? end_alpha_ : 0.0f },
                                   .target_out_alpha { ( target_out_alpha_ >= 0 ) ? target_out_alpha_ : 1.0f },
                                   .speed { ( speed_ >= 0 ) ? speed_ : 2.0f },
                                   .black_out_duration { ( black_out_duration_ >= 0 ) ? black_out_duration_ : 1.0f } };

      // type決定
      if ( type_ == "OutIn" ) { fade_comp.type = sdl_engine::Fade::Type::OutIn; }
      else if ( type_ == "Out" ) { fade_comp.type = sdl_engine::Fade::Type::Out; }
      else if ( type_ == "In" ) { fade_comp.type = sdl_engine::Fade::Type::In; }
      else
      {
         std::string err_msg = "EntityFactory::createDefaultFadeEntity: Invalid fade type ";
         err_msg += std::string( type_ );
         throw sdl_engine::GameException( err_msg.c_str() );
      }

      _registry.emplace<sdl_engine::Fade>( fade, fade_comp );

      // フラグでUIの下に描画するか最前面に描画するか決める
      if ( under_ui_ ) { _registry.emplace<sdl_engine::RenderFadeUnderUITag>( fade ); }
      else { _registry.emplace<sdl_engine::RenderFadeTag>( fade ); }

      return fade;
   }

   entt::entity
   EntityFactory::createSoundEffect( const std::string_view resource_key_, const i64 loop_count_, const f32 volume_ )
   {
      auto                    entity { _registry.create() };
      sdl_engine::SoundEffect sound_comp { .sound { _resource_manager.getSound( resource_key_ ) },
                                           .loop_count { loop_count_ },
                                           .volume { volume_ } };
      _registry.emplace<sdl_engine::SoundEffect>( entity, sound_comp );
      return entity;
   }

   entt::entity EntityFactory::createBGM( const std::string_view resource_key_,
                                          const i64              loop_count_,
                                          const i64              fade_time_ms_,
                                          const f32              volume_ )
   {
      auto                        entity { _registry.create() };
      sdl_engine::BackgroundMusic sound_comp { .sound { _resource_manager.getSound( resource_key_ ) },
                                               .loop_count { loop_count_ },
                                               .fade_time_ms { fade_time_ms_ },
                                               .volume { volume_ } };
      _registry.emplace<sdl_engine::BackgroundMusic>( entity, sound_comp );
      return entity;
   }

   entt::entity EntityFactory::createBullet( entt::entity& shooter_, const std::type_index& affiliation_id_ )
   {
      using namespace sdl_engine;

      auto entity { _registry.create() };

      // affiliation
      setAffiliationTag( entity, affiliation_id_ );

      _registry.emplace<EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      _registry.emplace<PlayerBulletTag>( entity );
      _registry.emplace<RenderGameSpriteTag>( entity );

      // [status]
      {
         Status status { .hp { 1 }, .max_hp { 1 }, .atk { 1 } };
         _registry.emplace<Status>( entity, status );
      }

      // [transform] shooterの位置をそのまま使う
      {
         auto trfm_comp { _registry.get<Transform>( shooter_ ) };
         _registry.emplace<Transform>( entity, trfm_comp );
      }
      // [velocity] shooterがvelocityを決める
      const auto& shooter_comp { _registry.get<Shooter>( shooter_ ) };
      {
         Velocity velo { shooter_comp.bullet_velocity.x, shooter_comp.bullet_velocity.y };
         _registry.emplace<Velocity>( entity, velo );
      }

      // sprite
      Sprite sprt_comp {};
      {
         switch ( shooter_comp.bullet_type )
         {
            case BulletType::Player : sprt_comp = createSprite( _resource_manager.getSprite( "player_bullet" ) ); break;

            case BulletType::Enemy_small :
               sprt_comp = createSprite( _resource_manager.getSprite( "enemy_bullet_small" ) );
               break;
            case BulletType::Enemy_Large :
               sprt_comp = createSprite( _resource_manager.getSprite( "player_bullet" ) );
               break;
         }
         _registry.emplace<Sprite>( entity, sprt_comp );
      }
      // bbox
      {
         auto        harf_w { sprt_comp.src.w / 2 };
         auto        harf_h { sprt_comp.src.h / 2 };
         BoundingBox bb_comp { harf_w, harf_h, harf_w, BoundingBox::State::None, BoundingBox::EnableAxis::ALL };
         _registry.emplace<BoundingBox>( entity, bb_comp );
      }
      return entity;
   }
   entt::entity EntityFactory::createHitEffect( entt::entity dead_entt_, const std::type_index& affiliation_id_ )
   {

      entt::resource<sdl_engine::SpriteResource>     sprite {};
      entt::resource<sdl_engine::SpriteAnimResource> anim {};

      if ( _registry.all_of<PlayerBulletTag>( dead_entt_ ) )
      {
         sprite = _resource_manager.getSprite( "player_bullet_hit" );
         anim   = _resource_manager.getSpriteAnim( "player_bullet_hit_anim" );
      }
      else if ( _registry.all_of<EnemyTag>( dead_entt_ ) )
      {
         sprite = _resource_manager.getSprite( "enemy_dead" );
         anim   = _resource_manager.getSpriteAnim( "enemy_dead_anim" );
      }
      else if ( _registry.all_of<PlayerTag>( dead_entt_ ) )
      {
         sprite = _resource_manager.getSprite( "player_dead" );
         anim   = _resource_manager.getSpriteAnim( "player_dead_anim" );
      }

      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );

      _registry.emplace<EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );

      // shooterの位置をそのまま使う
      auto trfm { _registry.get<sdl_engine::Transform>( dead_entt_ ) };
      _registry.emplace<sdl_engine::Transform>( entity, trfm );

      auto sprt { sdl_engine::createSprite( sprite ) };
      auto sprt_anim { sdl_engine::createSpriteAnim( anim ) };

      sprt.dst.w = static_cast<f32>( anim->frame_width );
      sprt.dst.h = static_cast<f32>( anim->frame_height );

      _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      _registry.emplace<sdl_engine::SpriteAnim>( entity, sprt_anim );

      return entity;
   }
   entt::entity EntityFactory::createPlayer( const json& data_, const std::type_index& affiliation_id_ )
   {

      auto entity { _registry.create() };

      setAffiliationTag( entity, affiliation_id_ );

      // リソース
      auto sprt_resource { _resource_manager.getSprite( "player" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "player_anim" ) };
      f32  frame_w { static_cast<f32>( sprt_anim_resource->frame_width ) };
      f32  frame_h { static_cast<f32>( sprt_anim_resource->frame_height ) };

      // タグコンポーネント
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
      _registry.emplace<PlayerTag>( entity );
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      // [Sprite]
      {
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         sprt.dst.w = frame_w;
         sprt.dst.h = frame_h;
         _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      }
      // [SpriteAnim]
      {
         sdl_engine::SpriteAnim sp_anim { sdl_engine::createSpriteAnim( sprt_anim_resource, 1u ) };
         _registry.emplace<sdl_engine::SpriteAnim>( entity, sp_anim );
      }
      // [Transform]
      {
         sdl_engine::Transform trfm {
            { 0.0f, 0.0f },
            0.0f, 1.0f
         };
         auto pos { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "pos" ) };
         // array生成に対応するためoffsetを足す
         trfm.position.x = pos[ 0 ];
         trfm.position.y = pos[ 1 ];
         _registry.emplace<sdl_engine::Transform>( entity, trfm );
      }
      // [Velocity]
      {
         sdl_engine::Velocity velo {
            { 0.0f, 10.0f },
            0.0f, 0.0f
         };
         auto dir { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "dir" ) };
         velo.vector.x = dir[ 0 ];
         velo.vector.y = dir[ 1 ];
         _registry.emplace<sdl_engine::Velocity>( entity, velo );
      }
      // [BBox]
      {
         BoundingBox box { createBoundingBox( frame_w / 2, frame_h / 2, 20.0f ) };
         _registry.emplace<BoundingBox>( entity, box );
      }
      // [Input]
      _registry.emplace<PlayerInput>( entity );
      // [Shooter]
      {
         f32     cooldown { sdl_engine::getOptionalData<f32>( data_, "shoot_cooldown", 0.2f ) };
         auto    dir { sdl_engine::getOptionalData<std::array<f32, 2>>( data_, "bullet_velocity", { 0.0f, -500.0f } ) };
         Shooter shtr {
            cooldown, 0.0f, { dir[ 0 ], dir[ 1 ] },
              BulletType::Player
         };
         _registry.emplace<Shooter>( entity, shtr );
      }
      // [status]
      {
         Status status { .hp { 3 }, .max_hp { 3 }, .atk { 0 } };
         _registry.emplace<Status>( entity, status );
      }
      // [damage effect prop]
      {
         auto red_brink_time = sdl_engine::getOptionalData<f32>( data_, "red_brink_time", 0.5f );
         auto brink_interval = sdl_engine::getOptionalData<f32>( data_, "brink_interval", 0.1f );

         DamageEffectProperty dmg_effect_prop { red_brink_time, brink_interval };
         _registry.emplace<DamageEffectProperty>( entity, dmg_effect_prop );
      }
      // [movement]
      {
         auto                max_speed { sdl_engine::getOptionalData<f32>( data_, "max_speed", 400.0f ) };
         auto                acceleration { sdl_engine::getOptionalData<f32>( data_, "acceleration", 5.0f ) };
         PlayerMovementInput move_input {
            max_speed, acceleration, { 0.f, 0.f }
         };
         _registry.emplace<PlayerMovementInput>( entity, move_input );
      }

      createPlayerBooster( entity, affiliation_id_ );

      return entity;
   }
   entt::entity EntityFactory::createPlayerBooster( const entt::entity parent_, const std::type_index& affiliation_id_ )
   {
      auto entity { _registry.create() };

      setAffiliationTag( entity, affiliation_id_ );

      // リソース
      auto sprt_resource { _resource_manager.getSprite( "player_booster" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "player_booster_anim" ) };
      f32  frame_w { static_cast<f32>( sprt_anim_resource->frame_width ) };
      f32  frame_h { static_cast<f32>( sprt_anim_resource->frame_height ) };

      // タグコンポーネント
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
      _registry.emplace<PlayerTag>( entity );
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      // [Sprite]
      {
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         sprt.dst.w = frame_w;
         sprt.dst.h = frame_h;
         _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      }
      // [SpriteAnim]
      {
         sdl_engine::SpriteAnim sp_anim { sdl_engine::createSpriteAnim( sprt_anim_resource, 1u ) };
         _registry.emplace<sdl_engine::SpriteAnim>( entity, sp_anim );
      }
      // [Transform]
      {
         sdl_engine::Transform trfm {
            { 0.0f, 0.0f },
            0.0f, 1.0f
         };
         _registry.emplace<sdl_engine::Transform>( entity, trfm );
      }
      // [TransformLink]
      {
         TransformLink trfm_link {
            parent_, { 0.0f, 40.0f }
         };
         _registry.emplace<TransformLink>( entity, trfm_link );
      }
      return entity;
   }
   entt::entity EntityFactory::createWandererEnemy( const json&             data_,
                                                    const std::type_index&  affiliation_id_,
                                                    sdl_engine::Vector2_f32 offset_pos_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      auto sprt_resource { _resource_manager.getSprite( "enemy1" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "enemy1_anim" ) };

      f32 frame_w { static_cast<f32>( sprt_anim_resource->frame_width ) };
      f32 frame_h { static_cast<f32>( sprt_anim_resource->frame_height ) };

      // タグ
      {
         _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
         _registry.emplace<EnemyTag>( entity );
         auto interval { sdl_engine::getOptionalData<f32>( data_, "interval", 0.0f, true ) };
         if ( interval > 0.0f ) { _registry.emplace<WaitTag>( entity, interval, 0.0f ); }
         else
         {
            _registry.emplace<EnteringTag>( entity );
            _registry.emplace<sdl_engine::RenderableTag>( entity );
            _registry.emplace<sdl_engine::UpdateableTag>( entity );
         }
      }
      // [Sprite]
      {
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         sprt.dst.w = frame_w;
         sprt.dst.h = frame_h;
         _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      }
      // [SpriteAnim]
      {
         sdl_engine::SpriteAnim sp_anim { sdl_engine::createSpriteAnim( sprt_anim_resource ) };
         _registry.emplace<sdl_engine::SpriteAnim>( entity, sp_anim );
      }
      // [Transform]
      sdl_engine::Transform trfm {
         { 0.0f, 0.0f },
         180.0f, 1.0f
      };
      {

         // array生成に対応するためoffsetを足す
         auto pos { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "pos" ) };
         trfm.position.x = pos[ 0 ] + offset_pos_.x;
         trfm.position.y = pos[ 1 ] + offset_pos_.y;
         _registry.emplace<sdl_engine::Transform>( entity, trfm );
      }
      // [Velocity]
      sdl_engine::Velocity velo {
         { 0.0f, 10.0f },
         0.0f, 0.0f
      };
      {
         auto dir { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "dir" ) };
         velo.vector.x = dir[ 0 ];
         velo.vector.y = dir[ 1 ];
         _registry.emplace<sdl_engine::Velocity>( entity, velo );
      }
      // [movement]
      {
         auto move_data { sdl_engine::tryGetJson( data_, "movement" ) };
         if ( move_data )
         {
            auto type { sdl_engine::getRequireData<std::string>( move_data.value(), "type" ) };
            if ( type == "serpentine" )
            {
               SerpentineMovement serpent {};
               serpent.center_x       = trfm.position.x;
               serpent.amplitude      = sdl_engine::getOptionalData<f32>( move_data.value(), "amplitude", 200.0f );
               serpent.frequency      = sdl_engine::getOptionalData<f32>( move_data.value(), "frequency", 20.0f );
               serpent.move_speed     = sdl_engine::getOptionalData<f32>( move_data.value(), "speed", 200.0f );
               serpent.move_threshold = sdl_engine::getOptionalData<f32>( move_data.value(), "threshold", 0.9f );
               serpent.time           = 0.0f;
               _registry.emplace<SerpentineMovement>( entity, serpent );
            }
            else if ( type == "sin_wave" )
            {
               SinWaveMovement sin_wave {};
               sin_wave.amplitude  = sdl_engine::getOptionalData<f32>( move_data.value(), "amplitude", 100.0f );
               sin_wave.frequency  = sdl_engine::getOptionalData<f32>( move_data.value(), "frequency", 10.0f );
               sin_wave.move_speed = sdl_engine::getOptionalData<f32>( move_data.value(), "speed", 200.0f );
               sin_wave.direction  = sdl_engine::Vector2_f32( velo.vector.x, velo.vector.y );
               sin_wave.time       = 0.0f;
               _registry.emplace<SinWaveMovement>( entity, sin_wave );
            }
         }
      }
      // [BBox]
      {
         BoundingBox box { createBoundingBox( frame_w / 2, frame_h / 2, 20.0f ) };
         _registry.emplace<BoundingBox>( entity, box );
      }
      // [Status]
      {
         Status status { .hp { 2 }, .max_hp { 2 }, .atk { 1 } };
         _registry.emplace<Status>( entity, status );
      }
      // [damage effect prop]
      {
         auto red_brink_time = sdl_engine::getOptionalData<f32>( data_, "red_brink_time", 0.5f );
         auto brink_interval = sdl_engine::getOptionalData<f32>( data_, "brink_interval", 0.5f / 7 );

         DamageEffectProperty dmg_effect_prop { red_brink_time, brink_interval };
         _registry.emplace<DamageEffectProperty>( entity, dmg_effect_prop );
      }
      return entity;
   }
   entt::entity EntityFactory::createBasicUI( const json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      auto sp_name { sdl_engine::getRequireData<std::string>( data_, "sprite_name" ) };
      sprt_resource = _resource_manager.getSprite( sp_name );

      // タグコンポーネント
      _registry.emplace<sdl_engine::RenderUITag>( entity );
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      // Sprite
      {
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      }
      // Transform
      {
         sdl_engine::Transform trfm {
            { 0.0f, 0.0f },
            0.0f, 1.0f
         };
         auto pos { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "pos" ) };
         // array生成に対応するためoffsetを足す
         trfm.position.x = pos[ 0 ];
         trfm.position.y = pos[ 1 ];
         _registry.emplace<sdl_engine::Transform>( entity, trfm );
      }
      return entity;
   }
   entt::entity EntityFactory::createHighlightableUI( const json& data_, const std::type_index& affiliation_id_ )
   {
      auto          entity { createBasicUI( data_, affiliation_id_ ) };    // selectable
      auto          sprt { _registry.get<sdl_engine::Sprite>( entity ) };
      auto          inactive_color { sprt.color * 0.5 };
      Highlightable selectable { false, sprt.color, inactive_color };
      _registry.emplace<Highlightable>( entity, selectable );
      return entity;
   }
   entt::entity EntityFactory::createBasicText( const json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      entt::resource<sdl_engine::FontResource>   font_resource {};
      auto font_size { sdl_engine::getRequireData<std::string>( data_, "font_size" ) };
      if ( font_size == "small" )
      {
         sprt_resource = _resource_manager.getSprite( "font_1_x8" );
         font_resource = _resource_manager.getFont( "font_x8" );
      }
      else if ( font_size == "medium" )
      {
         sprt_resource = _resource_manager.getSprite( "font_1_x16" );
         font_resource = _resource_manager.getFont( "font_x16" );
      }
      else if ( font_size == "large" )
      {
         sprt_resource = _resource_manager.getSprite( "font_1_x24" );
         font_resource = _resource_manager.getFont( "font_x24" );
      }
      // タグコンポーネント
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      _registry.emplace<sdl_engine::RenderTextTag>( entity );
      // Sprite
      {
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         auto color { sdl_engine::getOptionalData<std::array<f32, 4>>( data_, "color", { 1.0f, 1.0f, 1.0f, 1.0f } ) };
         sprt.color = sdl_engine::ColorRGBA( color );
         _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      }
      // Transform
      {
         sdl_engine::Transform trfm {
            { 0.0f, 0.0f },
            0.0f, 1.0f
         };
         auto pos { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "pos" ) };
         // array生成に対応するためoffsetを足す
         trfm.position.x = pos[ 0 ];
         trfm.position.y = pos[ 1 ];
         _registry.emplace<sdl_engine::Transform>( entity, trfm );
      }

      // Text
      {
         sdl_engine::Text text { font_resource, "none" };
         auto             text_data { sdl_engine::getRequireData<std::string>( data_, "text" ) };
         text.text = text_data;
         _registry.emplace<sdl_engine::Text>( entity, text );
      }

      return entity;
   }
   entt::entity EntityFactory::createBrinkText( const json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { createBasicText( data_, affiliation_id_ ) };

      // Brink
      auto        speed { sdl_engine::getOptionalData<f32>( data_, "brink_speed", 1.0f ) };
      auto        min_alpha { sdl_engine::getOptionalData<f32>( data_, "brink_min_alpha", 0.3f ) };
      SpriteBrink brink { speed, min_alpha };
      _registry.emplace<SpriteBrink>( entity, brink );

      return entity;
   }
   entt::entity EntityFactory::createHighlightableText( const json& data_, const std::type_index& affiliation_id_ )
   {

      auto entity { createBasicText( data_, affiliation_id_ ) };
      // selectable
      auto          sprt { _registry.get<sdl_engine::Sprite>( entity ) };
      auto          inactive_color { sprt.color * 0.5 };
      Highlightable selectable { false, sprt.color, inactive_color };
      auto          active_data { sdl_engine::getOptionalData<u32>( data_, "highlightable", false ) };
      selectable.active = active_data;
      _registry.emplace<Highlightable>( entity, selectable );
      return entity;
   }
   entt::entity EntityFactory::createTitleMenu( const json& data_, const std::type_index& affiliation_id_ )
   {
      entt::entity entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      TitleMenu menu { .menu_ui {}, .selected { 0 } };
      if ( auto highlight_ui { sdl_engine::tryGetJson( data_, "highlightable_ui" ) }; highlight_ui )
      {
         for ( auto& ui_data : highlight_ui->get() )
         {
            auto     ui_entt { createHighlightableUI( ui_data, affiliation_id_ ) };
            ButtonUI button_ui {};
            auto     button { sdl_engine::getRequireData<std::string>( ui_data, "button_type" ) };
            if ( button == "start" ) { button_ui.type = ButtonUI::Type::Start; }
            else if ( button == "exit" ) { button_ui.type = ButtonUI::Type::Exit; }
            _registry.emplace<ButtonUI>( ui_entt, button_ui );
            menu.menu_ui.emplace_back( ui_entt );
         }
      }

      _registry.emplace<TitleMenu>( entity, menu );
      _registry.emplace<TitleInput>( entity );
      return entity;
   }
   std::pair<entt::entity, entt::entity> EntityFactory::createBackGround( const json&            data_,
                                                                          const std::type_index& affiliation_id_ )
   {

      std::array<entt::entity, 2> entities { _registry.create(), _registry.create() };
      setAffiliationTag( entities[ 0 ], affiliation_id_ );
      setAffiliationTag( entities[ 1 ], affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      auto sp_name { sdl_engine::getRequireData<std::string>( data_, "sprite_name" ) };
      sprt_resource = _resource_manager.getSprite( sp_name );

      for ( u32 i { 0 }; auto& entt : entities )
      {
         // タグコンポーネント
         _registry.emplace<sdl_engine::RenderBackgroundTag>( entt );
         _registry.emplace<BackgroundTag>( entt );
         _registry.emplace<myge::EnteringTag>( entt );
         _registry.emplace<sdl_engine::RenderableTag>( entt );
         _registry.emplace<sdl_engine::UpdateableTag>( entt );
         // Sprite
         sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
         sprt.render_order = sdl_engine::getOptionalData<u32>( data_, "z_order", 100 );
         _registry.emplace<sdl_engine::Sprite>( entt, sprt );

         // Transform
         sdl_engine::Transform trfm {
            { 300.0f, 0.0f },
            0.0f, 1.0f
         };
         auto pos { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "pos" ) };
         trfm.position.x = pos[ 0 ];
         trfm.position.y = pos[ 1 ] - sprt.dst.h * i++;
         _registry.emplace<sdl_engine::Transform>( entt, trfm );

         // Velocity
         sdl_engine::Velocity velo {
            { 0.0f, 10.0f },
            0.0f, 0.0f
         };
         auto dir { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "dir" ) };
         velo.vector.x = dir[ 0 ];
         velo.vector.y = dir[ 1 ];
         _registry.emplace<sdl_engine::Velocity>( entt, velo );
         // BBox
         BoundingBox box { createBoundingBox( sprt.dst.w / 2, sprt.dst.h / 2, 0.0f, BoundingBox::EnableAxis::Top ) };
         _registry.emplace<BoundingBox>( entt, box );
      }

      return std::pair { entities[ 0 ], entities[ 1 ] };
   }
   std::vector<entt::entity> EntityFactory::createWandererEnemyArray( const json&            data_,
                                                                      const std::type_index& affiliation_id_ )
   {
      auto                      num_enemy { sdl_engine::getRequireData<u32>( data_, "num" ) };
      auto                      offset { sdl_engine::getRequireData<std::array<f32, 2>>( data_, "offset_pos" ) };
      std::vector<entt::entity> entities { num_enemy };

      for ( u32 i = 0; i < num_enemy; i++ )
      {
         sdl_engine::Vector2_f32 vec;
         vec.x         = offset[ 0 ] * i;
         vec.y         = offset[ 1 ] * i;
         entities[ i ] = createWandererEnemy( data_, affiliation_id_, vec );
      }
      return entities;
   }
   std::vector<entt::entity> EntityFactory::createEntities( const json&                     data_,
                                                            const std::type_index&          affiliation_id_,
                                                            const std::vector<std::string>& exclude_ )
   {

      std::vector<entt::entity> entities {};

      for ( auto& [ entity_type, data_array ] : data_.items() )
      {
         // 一斉生成から除外する対象があったら次のタイプへ進む
         for ( auto& exc : exclude_ )
         {
            if ( exc == entity_type ) { goto next_type; }
         }
         for ( auto& data : data_array )
         {
            if ( entity_type == "en_wanderer_array" )
            {
               auto entts { createWandererEnemyArray( data, affiliation_id_ ) };
               entities.insert(
                 entities.end(), std::make_move_iterator( entts.begin() ), std::make_move_iterator( entts.end() ) );
            }
            else if ( entity_type == "player" )
            {
               auto entt { createPlayer( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
            else if ( entity_type == "background" )
            {
               auto entts { createBackGround( data, affiliation_id_ ) };
               entities.emplace_back( entts.first );
               entities.emplace_back( entts.second );
            }
            else if ( entity_type == "basic_ui" )
            {
               auto entt { createBasicUI( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
            else if ( entity_type == "brink_text" )
            {
               auto entt { createBrinkText( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
            else if ( entity_type == "highlightable_text" )
            {
               auto entt { createHighlightableText( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
            else if ( entity_type == "highlightable_ui" )
            {
               auto entt { createHighlightableUI( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
            else if ( entity_type == "menu" )
            {
               auto entt { createTitleMenu( data, affiliation_id_ ) };
               entities.emplace_back( entt );
            }
         }

      next_type:;
      }

      return entities;
   }
   void EntityFactory::setAffiliationTag( entt::entity entity_, const std::type_index& affiliation_id_ )
   {
      if ( affiliation_id_ == typeid( AffilTitleScene ) ) { _registry.emplace<AffilTitleScene>( entity_ ); }
      else if ( affiliation_id_ == typeid( AffilGameScene ) ) { _registry.emplace<AffilGameScene>( entity_ ); }
   }
}    // namespace myge