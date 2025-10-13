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
   entt::entity EntityFactory::createBullet( entt::registry&              registry_,
                                             sdl_engine::ResourceManager& resource_manager_,
                                             entt::entity&                shooter_ )
   {
      using namespace sdl_engine;

      auto bullet_entt { registry_.create() };

      registry_.emplace<EnteringTag>( bullet_entt );

      // shooterの位置をそのまま使う
      auto trfm_comp { registry_.get<Transform>( shooter_ ) };
      registry_.emplace<Transform>( bullet_entt, trfm_comp );

      // shooterがvelocityを決める
      auto     shooter_comp { registry_.get<Shooter>( shooter_ ) };
      Velocity velo { shooter_comp.bullet_velocity.x, shooter_comp.bullet_velocity.y };
      registry_.emplace<Velocity>( bullet_entt, velo );

      // sprite
      Sprite sprt_comp {};
      switch ( shooter_comp.bullet_type )
      {
         case BulletType::Player : sprt_comp = createSprite( resource_manager_.getSprite( "player_bullet" ) ); break;
         case BulletType::Enemy_small :
            sprt_comp = createSprite( resource_manager_.getSprite( "enemy_bullet_small" ) );
            break;
         case BulletType::Enemy_Large :
            sprt_comp = createSprite( resource_manager_.getSprite( "player_bullet" ) );
            break;
      }
      registry_.emplace<Sprite>( bullet_entt, sprt_comp );

      //
      auto        harf_w { sprt_comp.src.w / 2 };
      auto        harf_h { sprt_comp.src.h / 2 };
      BoundingBox bb_comp { harf_w, harf_h, harf_w, BoundingBox::State::None, BoundingBox::EnableAxis::ALL };
      registry_.emplace<BoundingBox>( bullet_entt, bb_comp );

      registry_.emplace<EnemyTag>( bullet_entt );
      registry_.emplace<RenderGameSpriteTag>( bullet_entt );
      return bullet_entt;
   }
   std::vector<entt::entity> EntityFactory::createEntities( entt::registry&              registry_,
                                                            sdl_engine::ResourceManager& resource_manager_,
                                                            json&                        data_ )
   {

      std::vector<entt::entity> entities { data_.size() };

      // 取得したデータからエンティティを生成
      for ( i32 i { 0 }; auto& entity_data : data_ )
      {
         entities[ i ] = registry_.create();
         // ライフサイクルタグ [ 画面外からゲームエリアへ向かう ]状態を初期値とする
         registry_.emplace<myge::EnteringTag>( entities[ i ] );
         // 基本的なコンポーネント
         // [Transform]
         if ( auto trfm_data { sdl_engine::getJsonData<json>( entity_data, "Transform" ) }; trfm_data )
         {
            auto trfm_comp { sdl_engine::createTransform( trfm_data ) };
            registry_.emplace<sdl_engine::Transform>( entities[ i ], trfm_comp );
         }
         else
         {
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION,
                          "EntityデータにTransformが設定されていませんでした.初期値で生成します" );
            registry_.emplace<sdl_engine::Transform>( entities[ i ], sdl_engine::createTransform() );
         }
         // [Sprite]
         if ( auto sprt_data { sdl_engine::getJsonData<json>( entity_data, "Sprite" ) }; sprt_data )
         {
            auto sprt_comp = sdl_engine::createSprite( resource_manager_, sprt_data );
            registry_.emplace<sdl_engine::Sprite>( entities[ i ], sprt_comp );
         }
         // [Velocity]
         if ( auto vlcy_data { sdl_engine::getJsonData<json>( entity_data, "Velocity" ) }; vlcy_data )
         {
            auto vlcy_comp = sdl_engine::createVelocity( vlcy_data );
            registry_.emplace<sdl_engine::Velocity>( entities[ i ], vlcy_comp );
         }
         // [SpriteAnim]
         if ( auto anim_data { sdl_engine::getJsonData<json>( entity_data, "SpriteAnim" ) }; anim_data )
         {
            // SpriteAnimコンポーネントを作成
            auto sprt_anim_comp = sdl_engine::createSpriteAnim( resource_manager_, anim_data );
            // SpriteAnimが存在するということは、Spriteも存在する
            // SpriteのdstをSpriteAnimのフレームサイズに設定しておく
            auto& sprt = registry_.get<sdl_engine::Sprite>( entities[ i ] );
            sprt.dst.w = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_width );
            sprt.dst.h = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_height );
            // SpriteAnimをエンティティに登録
            registry_.emplace<sdl_engine::SpriteAnim>( entities[ i ], sprt_anim_comp );
         }

         // [PlayerInput]
         if ( auto pl_in_data { sdl_engine::getJsonData<json>( entity_data, "PlayerInput" ) }; pl_in_data )
         {
            registry_.emplace<PlayerInput>( entities[ i ], createPlayerInput() );
         }
         // [BoundingBox]
         if ( auto bb_data { sdl_engine::getJsonData<json>( entity_data, "BoundingBox" ) }; bb_data )
         {
            BoundingBox box_comp { createBoundingBox( bb_data ) };

            registry_.emplace<myge::BoundingBox>( entities[ i ], box_comp );
         }
         // [TitleInput]
         if ( entity_data.contains( "TitleInput" ) ) { registry_.emplace<TitleInput>( entities[ i ] ); }
         // [SpriteBrink]
         if ( auto brink_data { sdl_engine::getJsonData<json>( entity_data, "SpriteBrink" ) }; brink_data )
         {
            SpriteBrink brink_comp { createSpriteBrink( brink_data ) };
            registry_.emplace<SpriteBrink>( entities[ i ], brink_comp );
         }
         // [SerpentineMovement]
         if ( auto serpentine_data { sdl_engine::getJsonData<json>( entity_data, "SerpentineMovement" ) };
              serpentine_data )
         {
            SerpentineMovement serpentine_comp { createSerpentineMovement( serpentine_data ) };
            registry_.emplace<SerpentineMovement>( entities[ i ], serpentine_comp );
            // 直接Transformを変更するため必要なタグ
            registry_.emplace<sdl_engine::DirectRotateTag>( entities[ i ] );
         }
         // [Text]
         if ( auto text_data { sdl_engine::getJsonData<json>( entity_data, "Text" ) }; text_data )
         {
            sdl_engine::Text text_comp { sdl_engine::createText( resource_manager_, text_data ) };
            registry_.emplace<sdl_engine::Text>( entities[ i ], text_comp );
         }
         // [Fade]
         if ( auto fade_data { sdl_engine::getJsonData<json>( entity_data, "Fade" ) }; fade_data )
         {
            sdl_engine::Fade fade_comp { sdl_engine::createFade( fade_data ) };
            registry_.emplace<sdl_engine::Fade>( entities[ i ], fade_comp );
         }
         // [Shooter]
         if ( auto shtr_data { sdl_engine::getJsonData<json>( entity_data, "Shooter" ) }; shtr_data )
         {
            Shooter shtr_comp { createShooter( shtr_data ) };
            registry_.emplace<Shooter>( entities[ i ], shtr_comp );
         }
         // [EntityType]
         if ( auto entity_type { sdl_engine::getJsonData<std::string>( entity_data, "EntityType" ) }; entity_type )
         {
            emplaceEntityTypeTag( registry_, entities[ i ], entity_type.value() );
         }
         // [RenderType]
         if ( auto render_type { sdl_engine::getJsonData<std::string>( entity_data, "RenderType" ) }; render_type )
         {
            sdl_engine::emplaceRenderTypeTag( registry_, entities[ i ], render_type.value() );
         }
         else    // デフォルトのレンダータイプはGameSpriteとする
         {
            registry_.emplace<sdl_engine::RenderGameSpriteTag>( entities[ i ] );
         }
         i++;
      }

      return entities;
   }
}    // namespace myge