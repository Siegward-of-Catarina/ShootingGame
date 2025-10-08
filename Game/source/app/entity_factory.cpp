#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
// basic_components
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
namespace
{
   // Spriteのリソースを取得し、エンティティにSpriteを登録
   void getSpriteResourceAndEmplaceComponent( entt::registry&              registry_,
                                              sdl_engine::ResourceManager& resource_manager_,
                                              entt::entity                 entity_,
                                              const json&                  sprite_data_ )
   {    // SpriteResource取得
      auto sprite_name { sdl_engine::getJsonData<std::string>( sprite_data_, "name" ) };
      auto sprite = resource_manager_.getSprite( sprite_name.value() );
      auto render_order { sdl_engine::getJsonData<u32>( sprite_data_, "render_order" ) };
      // Spriteをエンティティに登録
      auto sprt_comp = createSprite( sprite, render_order );
      registry_.emplace<sdl_engine::Sprite>( entity_, sprt_comp );
   }

   // SpriteAnimのリソースを取得し、エンティティにSpriteAnimを登録
   void getSpriteAnimResourceAndEmplaceComponent( entt::registry&              registry_,
                                                  sdl_engine::ResourceManager& resource_manager_,
                                                  entt::entity                 entity_,
                                                  const json&                  sprite_data_ )
   {
      // SpriterAnimResoruce取得
      auto sprite_anim_name { sdl_engine::getJsonData<std::string>( sprite_data_, "name" ) };
      auto sprt_anim_data = resource_manager_.getSpriteAnim( sprite_anim_name.value() );

      // SpriteAnimが存在するということは、Spriteも存在するはず
      // SpriteのdstをSpriteAnimのフレームサイズに設定しておく
      auto& sprt = registry_.get<sdl_engine::Sprite>( entity_ );
      sprt.dst.w = static_cast<f32>( sprt_anim_data->frame_width );
      sprt.dst.h = static_cast<f32>( sprt_anim_data->frame_height );

      // SpriteAnimコンポーネントを作成
      auto sprt_anim_comp = sdl_engine::createSpriteAnim( sprt_anim_data, sprite_data_ );
      // SpriteAnimをエンティティに登録
      registry_.emplace<sdl_engine::SpriteAnim>( entity_, sprt_anim_comp );
   }

   void emplaceBasicComponents( entt::registry&              registry_,
                                sdl_engine::ResourceManager& resource_manager_,
                                entt::entity                 entity_,
                                const json&                  data_ )
   {
      // [ Sprite取得 ]
      if ( !data_.contains( "Sprite" ) )
      {
         throw sdl_engine::GameException( "Entityデータにspriteが含まれていません" );
      }
      getSpriteResourceAndEmplaceComponent( registry_, resource_manager_, entity_, data_.at( "Sprite" ) );

      // [ Transform取得 ]
      if ( !data_.contains( "Transform" ) )
      {
         throw sdl_engine::GameException( "Entityデータにtransformが含まれていません" );
      }
      auto trfm_comp = sdl_engine::createTransform( data_.at( "Transform" ) );
      registry_.emplace<sdl_engine::Transform>( entity_, trfm_comp );

      // [ Velocity取得 ]
      if ( !data_.contains( "Velocity" ) )
      {
         throw sdl_engine::GameException( "Entityデータにvelocityが含まれていません" );
      }
      auto vlcy_comp = sdl_engine::createVelocity( data_.at( "Velocity" ) );
      registry_.emplace<sdl_engine::Velocity>( entity_, vlcy_comp );
   }
}    // namespace
namespace myge
{
   std::vector<entt::entity> EntityFactory::createEntities( sdl_engine::GameContext& context_, json& data_ )
   {

      auto& registry { context_.getRegistry() };
      auto& resource_manager { context_.getResourceManager() };

      std::vector<json> entities_data;
      entities_data.reserve( 20 );    // 適当な数で予約

      // jsonからエンティティ単位でデータを取り出し、vectorへ
      if ( data_.contains( "Player" ) ) { entities_data.emplace_back( data_.at( "Player" ) ); }

      if ( data_.contains( "Enemy" ) )
      {
         for ( auto& enemy_data : data_.at( "Enemy" ) ) { entities_data.emplace_back( enemy_data ); }
      }

      if ( data_.contains( "BackGround" ) )
      {
         for ( auto& bg_data : data_.at( "BackGround" ) ) { entities_data.emplace_back( bg_data ); };
      }
      if ( data_.contains( "UI" ) )
      {
         for ( auto& hud_data : data_.at( "UI" ) ) { entities_data.emplace_back( hud_data ); }
      }
      std::vector<entt::entity> entities { entities_data.size() };

      // 取得したデータからエンティティを生成
      for ( i32 i { 0 }; auto& entity_data : entities_data )
      {
         entities[ i ] = registry.create();
         // ライフサイクルタグ [ 画面外からゲームエリアへ向かう ]状態を初期値とする
         registry.emplace<myge::EnteringTag>( entities[ i ] );
         // 基本的なコンポーネント
         emplaceBasicComponents( registry, resource_manager, entities[ i ], entity_data );
         // [spriteAnim]
         if ( entity_data.contains( "SpriteAnim" ) )
         {
            getSpriteAnimResourceAndEmplaceComponent(
              registry, resource_manager, entities[ i ], entity_data.at( "SpriteAnim" ) );
         }
         // [playerInput]
         if ( entity_data.contains( "PlayerInput" ) )
         {
            auto& data = entity_data.at( "PlayerInput" );

            PlayerInput input_comp { createPlayerInput( data ) };
            registry.emplace<PlayerInput>( entities[ i ], input_comp );
         }
         // [BoundingBox]
         if ( entity_data.contains( "BoundingBox" ) )
         {
            auto&             data = entity_data.at( "BoundingBox" );
            myge::BoundingBox box_comp { createBoundingBox( data ) };

            registry.emplace<myge::BoundingBox>( entities[ i ], box_comp );
         }
         // [TitleInput]
         if ( entity_data.contains( "TitleInput" ) ) { registry.emplace<TitleInput>( entities[ i ] ); }
         // [EntityType]
         auto entity_type { sdl_engine::getJsonData<std::string>( entity_data, "EntityType" ) };
         if ( entity_type ) { emplaceEntityTypeTag( registry, entities[ i ], entity_type.value() ); }
         // [RenderType]
         auto render_type { sdl_engine::getJsonData<std::string>( entity_data, "RenderType" ) };
         if ( render_type ) { sdl_engine::emplaceRenderTypeTag( registry, entities[ i ], render_type.value() ); }
         // デフォルトのレンダータイプはGameSpriteとする
         else { registry.emplace<sdl_engine::RenderGameSpriteTag>( entities[ i ] ); }
         i++;
      }

      return entities;
   }
}    // namespace myge