#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/components/shooter.hpp>
#include <app/components/sprite_brink.hpp>
#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
// basic_components
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
namespace myge
{
   EntityFactory::EntityFactory( entt::registry& registry_, sdl_engine::ResourceManager& resource_manager_ )
     : _registry { registry_ }, _resource_manager { resource_manager_ }
   {
   }
   EntityFactory::~EntityFactory() {}
   entt::entity EntityFactory::createDefaultFadeEntity( f32 window_width, f32 window_height )
   {
      auto fade { _registry.create() };
      // transfrom
      sdl_engine::Transform trfm_cmp {
         .x { window_width / 2.0f }, .y { window_height / 2.0f }, .angle { 0.0f }, .scale { 1.0f }
      };
      _registry.emplace<sdl_engine::Transform>( fade, trfm_cmp );
      // sprite
      sdl_engine::Sprite sprt_comp {};
      sprt_comp.texture = _resource_manager.getSprite( "white" );
      sprt_comp.src     = { 0.0f, 0.0f, window_width, window_height };
      sprt_comp.dst     = { trfm_cmp.x, trfm_cmp.y, window_width, window_height };
      sprt_comp.color   = { 0.0f, 0.0f, 0.0f, 0.0f };
      _registry.emplace<sdl_engine::Sprite>( fade, sprt_comp );
      // fade
      sdl_engine::Fade fade_comp { .state { sdl_engine::Fade::State::Idel },
                                   .speed { 2.0f },
                                   .black_out_duration { 1.0f } };
      _registry.emplace<sdl_engine::Fade>( fade, fade_comp );
      _registry.emplace<sdl_engine::RenderFadeTag>( fade );
      return fade;
   }
   entt::entity EntityFactory::createBullet( entt::entity& shooter_ )
   {
      using namespace sdl_engine;

      auto bullet_entt { _registry.create() };

      _registry.emplace<EnteringTag>( bullet_entt );

      // shooterの位置をそのまま使う
      auto trfm_comp { _registry.get<Transform>( shooter_ ) };
      _registry.emplace<Transform>( bullet_entt, trfm_comp );

      // shooterがvelocityを決める
      const auto& shooter_comp { _registry.get<Shooter>( shooter_ ) };
      Velocity    velo { shooter_comp.bullet_velocity.x, shooter_comp.bullet_velocity.y };
      _registry.emplace<Velocity>( bullet_entt, velo );

      // sprite
      Sprite sprt_comp {};
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
      _registry.emplace<Sprite>( bullet_entt, sprt_comp );

      //
      auto        harf_w { sprt_comp.src.w / 2 };
      auto        harf_h { sprt_comp.src.h / 2 };
      BoundingBox bb_comp { harf_w, harf_h, harf_w, BoundingBox::State::None, BoundingBox::EnableAxis::ALL };
      _registry.emplace<BoundingBox>( bullet_entt, bb_comp );

      _registry.emplace<EnemyTag>( bullet_entt );
      _registry.emplace<RenderGameSpriteTag>( bullet_entt );
      return bullet_entt;
   }
   std::vector<entt::entity> EntityFactory::createEntities( json& data_ )
   {

      std::vector<entt::entity> entities { data_.size() };

      // 取得したデータからエンティティを生成
      for ( i32 i { 0 }; auto& entity_data : data_ )
      {
         entities[ i ] = _registry.create();
         // ライフサイクルタグ [ 画面外からゲームエリアへ向かう ]状態を初期値とする
         _registry.emplace<myge::EnteringTag>( entities[ i ] );
         // 基本的なコンポーネント
         // [Transform]
         if ( auto trfm_data { sdl_engine::getJsonData<json>( entity_data, "Transform" ) }; trfm_data )
         {
            auto trfm_comp { sdl_engine::createTransform( trfm_data ) };
            _registry.emplace<sdl_engine::Transform>( entities[ i ], trfm_comp );
         }
         else
         {
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION,
                          "EntityデータにTransformが設定されていませんでした.初期値で生成します" );
            _registry.emplace<sdl_engine::Transform>( entities[ i ], sdl_engine::createTransform() );
         }
         // [Sprite]
         if ( auto sprt_data { sdl_engine::getJsonData<json>( entity_data, "Sprite" ) }; sprt_data )
         {
            auto sprt_comp = sdl_engine::createSprite( _resource_manager, sprt_data );
            _registry.emplace<sdl_engine::Sprite>( entities[ i ], sprt_comp );
         }
         // [Velocity]
         if ( auto vlcy_data { sdl_engine::getJsonData<json>( entity_data, "Velocity" ) }; vlcy_data )
         {
            auto vlcy_comp = sdl_engine::createVelocity( vlcy_data );
            _registry.emplace<sdl_engine::Velocity>( entities[ i ], vlcy_comp );
         }
         // [SpriteAnim]
         if ( auto anim_data { sdl_engine::getJsonData<json>( entity_data, "SpriteAnim" ) }; anim_data )
         {
            // SpriteAnimコンポーネントを作成
            auto sprt_anim_comp = sdl_engine::createSpriteAnim( _resource_manager, anim_data );
            // SpriteAnimが存在するということは、Spriteも存在する
            // SpriteのdstをSpriteAnimのフレームサイズに設定しておく
            auto& sprt = _registry.get<sdl_engine::Sprite>( entities[ i ] );
            sprt.dst.w = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_width );
            sprt.dst.h = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_height );
            // SpriteAnimをエンティティに登録
            _registry.emplace<sdl_engine::SpriteAnim>( entities[ i ], sprt_anim_comp );
         }

         // [PlayerInput]
         if ( auto pl_in_data { sdl_engine::getJsonData<json>( entity_data, "PlayerInput" ) }; pl_in_data )
         {
            _registry.emplace<PlayerInput>( entities[ i ], createPlayerInput() );
         }
         // [BoundingBox]
         if ( auto bb_data { sdl_engine::getJsonData<json>( entity_data, "BoundingBox" ) }; bb_data )
         {
            BoundingBox box_comp { createBoundingBox( bb_data ) };

            _registry.emplace<myge::BoundingBox>( entities[ i ], box_comp );
         }
         // [TitleInput]
         if ( entity_data.contains( "TitleInput" ) ) { _registry.emplace<TitleInput>( entities[ i ] ); }
         // [SpriteBrink]
         if ( auto brink_data { sdl_engine::getJsonData<json>( entity_data, "SpriteBrink" ) }; brink_data )
         {
            SpriteBrink brink_comp { createSpriteBrink( brink_data ) };
            _registry.emplace<SpriteBrink>( entities[ i ], brink_comp );
         }
         // [SerpentineMovement]
         if ( auto serpentine_data { sdl_engine::getJsonData<json>( entity_data, "SerpentineMovement" ) };
              serpentine_data )
         {
            SerpentineMovement serpentine_comp { createSerpentineMovement( serpentine_data ) };
            _registry.emplace<SerpentineMovement>( entities[ i ], serpentine_comp );
            // 直接Transformを変更するため必要なタグ
            _registry.emplace<sdl_engine::DirectRotateTag>( entities[ i ] );
         }
         // [Text]
         if ( auto text_data { sdl_engine::getJsonData<json>( entity_data, "Text" ) }; text_data )
         {
            sdl_engine::Text text_comp { sdl_engine::createText( _resource_manager, text_data ) };
            _registry.emplace<sdl_engine::Text>( entities[ i ], text_comp );
         }
         // [Fade]
         if ( auto fade_data { sdl_engine::getJsonData<json>( entity_data, "Fade" ) }; fade_data )
         {
            sdl_engine::Fade fade_comp { sdl_engine::createFade( fade_data ) };
            _registry.emplace<sdl_engine::Fade>( entities[ i ], fade_comp );
         }
         // [Shooter]
         if ( auto shtr_data { sdl_engine::getJsonData<json>( entity_data, "Shooter" ) }; shtr_data )
         {
            Shooter shtr_comp { createShooter( shtr_data ) };
            _registry.emplace<Shooter>( entities[ i ], shtr_comp );
         }
         // [EntityType]
         if ( auto entity_type { sdl_engine::getJsonData<std::string>( entity_data, "EntityType" ) }; entity_type )
         {
            emplaceEntityTypeTag( _registry, entities[ i ], entity_type.value() );
         }
         // [RenderType]
         if ( auto render_type { sdl_engine::getJsonData<std::string>( entity_data, "RenderType" ) }; render_type )
         {
            sdl_engine::emplaceRenderTypeTag( _registry, entities[ i ], render_type.value() );
         }
         else    // デフォルトのレンダータイプはGameSpriteとする
         {
            _registry.emplace<sdl_engine::RenderGameSpriteTag>( entities[ i ] );
         }
         i++;
      }

      return entities;
   }
   std::vector<entt::entity> EntityFactory::createWandererEnemyArray( json& data_ )
   {
      auto                      num_enemy { sdl_engine::getJsonData<u32>( data_, "num" ) };
      auto                      offset { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "offset" ) };
      std::vector<entt::entity> entities { num_enemy.value() };

      for ( u32 i = 0; i < num_enemy.value(); i++ )
      {
         sdl_engine::Vector2_f32 vec;
         vec.x         = offset.value()[ 0 ] * i;
         vec.y         = offset.value()[ 1 ] * i;
         entities[ i ] = createWandererEnemy( data_, vec );
      }
      return entities;
   }
   entt::entity EntityFactory::createWandererEnemy( json& data_, sdl_engine::Vector2_f32 offset_pos_ )
   {
      auto entity { _registry.create() };
      // リソース
      auto sprt_resource { _resource_manager.getSprite( "enemy1" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "enemy1_anim" ) };
      // タグ
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
      _registry.emplace<EnemyTag>( entity );
      _registry.emplace<EnteringTag>( entity );

      sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
      sprt.dst.w = static_cast<f32>( sprt_anim_resource->frame_width );
      sprt.dst.h = static_cast<f32>( sprt_anim_resource->frame_height );
      _registry.emplace<sdl_engine::Sprite>( entity, sprt );

      sdl_engine::SpriteAnim sp_anim { sdl_engine::createSpriteAnim( sprt_anim_resource ) };
      _registry.emplace<sdl_engine::SpriteAnim>( entity, sp_anim );

      sdl_engine::Transform trfm { 0.0f, 0.0f, 180.0f, 1.0f };
      if ( auto pos { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "pos" ) }; pos )
      {
         // array生成に対応するためoffsetを足す
         trfm.x = pos.value()[ 0 ] + offset_pos_.x;
         trfm.y = pos.value()[ 1 ] + offset_pos_.y;
      }
      _registry.emplace<sdl_engine::Transform>( entity, trfm );

      sdl_engine::Velocity velo { 0.0f, 10.0f, 0.0f, 0.0f };
      if ( auto dir { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "dir" ) }; dir )
      {
         velo.dx = dir.value()[ 0 ];
         velo.dy = dir.value()[ 1 ];
      }
      _registry.emplace<sdl_engine::Velocity>( entity, velo );

      auto move_data { sdl_engine::getJsonData<json>( data_, "movement" ) };
      if ( move_data )
      {
         auto data { move_data.value() };
         auto type { sdl_engine::getJsonData<std::string>( data, "type" ) };
         if ( type == "serpentine" )
         {
            SerpentineMovement serpent {
               .center_x { trfm.x },
               .amplitude { data.value( "amplitude", 200.0f ) },
               .frequency { data.value( "frequency", 2.0f ) },
               .move_speed { data.value( "speed", 200.0f ) },
               .move_threshold { data.value( "threshold", 0.9f ) },
            };
            _registry.emplace<SerpentineMovement>( entity, serpent );
         }
      }

      BoundingBox box { createBoundingBox( sprt.dst.w / 2, sprt.dst.h / 2, 20.0f ) };
      _registry.emplace<BoundingBox>( entity, box );

      return entity;
   }
}    // namespace myge