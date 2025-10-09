#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/sprite_brink.hpp>
#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
// basic_components
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
namespace myge
{
   std::vector<entt::entity> EntityFactory::createEntities( sdl_engine::GameContext& context_, json& data_ )
   {

      auto& registry { context_.getRegistry() };
      auto& resource_manager { context_.getResourceManager() };

      std::vector<entt::entity> entities { data_.size() };

      // 取得したデータからエンティティを生成
      for ( i32 i { 0 }; auto& entity_data : data_ )
      {
         entities[ i ] = registry.create();
         // ライフサイクルタグ [ 画面外からゲームエリアへ向かう ]状態を初期値とする
         registry.emplace<myge::EnteringTag>( entities[ i ] );
         // 基本的なコンポーネント
         // [Transform]
         if ( auto trfm_data { sdl_engine::getJsonData<json>( entity_data, "Transform" ) }; trfm_data )
         {
            auto trfm_comp { sdl_engine::createTransform( trfm_data ) };
            registry.emplace<sdl_engine::Transform>( entities[ i ], trfm_comp );
         }
         else
         {
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION,
                          "EntityデータにTransformが設定されていませんでした.初期値で生成します" );
            registry.emplace<sdl_engine::Transform>( entities[ i ], sdl_engine::createTransform() );
         }
         // [Sprite]
         if ( auto sprt_data { sdl_engine::getJsonData<json>( entity_data, "Sprite" ) }; sprt_data )
         {
            auto sprt_comp = sdl_engine::createSprite( resource_manager, sprt_data );
            registry.emplace<sdl_engine::Sprite>( entities[ i ], sprt_comp );
         }
         // [Velocity]
         if ( auto vlcy_data { sdl_engine::getJsonData<json>( entity_data, "Velocity" ) }; vlcy_data )
         {
            auto vlcy_comp = sdl_engine::createVelocity( vlcy_data );
            registry.emplace<sdl_engine::Velocity>( entities[ i ], vlcy_comp );
         }
         // [SpriteAnim]
         if ( auto anim_data { sdl_engine::getJsonData<json>( entity_data, "SpriteAnim" ) }; anim_data )
         {
            // SpriteAnimコンポーネントを作成
            auto sprt_anim_comp = sdl_engine::createSpriteAnim( resource_manager, anim_data );
            // SpriteAnimが存在するということは、Spriteも存在する
            // SpriteのdstをSpriteAnimのフレームサイズに設定しておく
            auto& sprt = registry.get<sdl_engine::Sprite>( entities[ i ] );
            sprt.dst.w = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_width );
            sprt.dst.h = static_cast<f32>( sprt_anim_comp.sprite_anim->frame_height );
            // SpriteAnimをエンティティに登録
            registry.emplace<sdl_engine::SpriteAnim>( entities[ i ], sprt_anim_comp );
         }
         // [PlayerInput]
         if ( auto pl_in_data { sdl_engine::getJsonData<json>( entity_data, "PlayerInput" ) }; pl_in_data )
         {
            PlayerInput input_comp { createPlayerInput( pl_in_data ) };
            registry.emplace<PlayerInput>( entities[ i ], input_comp );
         }
         // [BoundingBox]
         if ( auto bb_data { sdl_engine::getJsonData<json>( entity_data, "BoundingBox" ) }; bb_data )
         {
            BoundingBox box_comp { createBoundingBox( bb_data ) };

            registry.emplace<myge::BoundingBox>( entities[ i ], box_comp );
         }
         // [TitleInput]
         if ( entity_data.contains( "TitleInput" ) ) { registry.emplace<TitleInput>( entities[ i ] ); }
         // [Text]
         if ( auto text_data { sdl_engine::getJsonData<json>( entity_data, "Text" ) }; text_data )
         {
            sdl_engine::Text text_comp { sdl_engine::createText( resource_manager, text_data ) };
            registry.emplace<sdl_engine::Text>( entities[ i ], text_comp );
         }
         // [SpriteBrink]
         if ( auto brink_data { sdl_engine::getJsonData<json>( entity_data, "SpriteBrink" ) }; brink_data )
         {
            SpriteBrink brink_comp { createSpriteBrink( brink_data ) };
            registry.emplace<SpriteBrink>( entities[ i ], brink_comp );
         }
         // [EntityType]
         if ( auto entity_type { sdl_engine::getJsonData<std::string>( entity_data, "EntityType" ) }; entity_type )
         {
            emplaceEntityTypeTag( registry, entities[ i ], entity_type.value() );
         }
         // [RenderType]
         if ( auto render_type { sdl_engine::getJsonData<std::string>( entity_data, "RenderType" ) }; render_type )
         {
            sdl_engine::emplaceRenderTypeTag( registry, entities[ i ], render_type.value() );
         }
         // デフォルトのレンダータイプはGameSpriteとする
         else { registry.emplace<sdl_engine::RenderGameSpriteTag>( entities[ i ] ); }
         i++;
      }

      return entities;
   }
}    // namespace myge