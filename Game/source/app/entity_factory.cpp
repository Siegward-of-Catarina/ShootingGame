#include <app/components/bounding_box.hpp>
#include <app/components/enemy_tag.hpp>
#include <app/components/lifecycle.hpp>
#include <app/components/out_of_screen_behavior.hpp>
#include <app/components/player_input.hpp>
#include <app/components/player_movement.hpp>
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
      auto sprite = resource_manager_.getSprite( sprite_name );

      // Spriteをエンティティに登録
      auto sprt_comp = getSpriteComponent( sprite );
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
      auto sprt_anim_data = resource_manager_.getSpriteAnim( sprite_anim_name );

      // SpriteAnimが存在するということは、Spriteも存在するはず
      // SpriteのdstをSpriteAnimのフレームサイズに設定しておく
      auto& sprt = registry_.get<sdl_engine::Sprite>( entity_ );
      sprt.dst.w = static_cast<f32>( sprt_anim_data->frame_width );
      sprt.dst.h = static_cast<f32>( sprt_anim_data->frame_height );

      // SpriteAnimコンポーネントを作成
      auto sprt_anim_comp = sdl_engine::getSpriteAnimComponent( sprt_anim_data, sprite_data_ );
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
      auto trfm_comp = sdl_engine::getTransFormComponent( data_.at( "Transform" ) );
      registry_.emplace<sdl_engine::Transform>( entity_, trfm_comp );

      // [ Velocity取得 ]
      if ( !data_.contains( "Velocity" ) )
      {
         throw sdl_engine::GameException( "Entityデータにvelocityが含まれていません" );
      }
      auto vlcy_comp = sdl_engine::getVelocityComponent( data_.at( "Velocity" ) );
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
      if ( data_.contains( "Title" ) ) { entities_data.emplace_back( data_.at( "Title" ) ); }
      std::vector<entt::entity> entities { entities_data.size() };

      // 取得したデータからエンティティを生成
      for ( i32 i { 0 }; auto& entity_data : entities_data )
      {
         entities[ i ] = registry.create();
         // ライフサイクルタグ [ 画面外からゲームエリアへ向かう ]状態を初期値とする
         // この時点で更新対象にもする
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

            PlayerInput input_comp {
               .up_sdl_key_name { sdl_engine::getJsonData<std::string>( data, "up_sdl_key_name" ) },
               .down_sdl_key_name { sdl_engine::getJsonData<std::string>( data, "down_sdl_key_name" ) },
               .left_sdl_key_name { sdl_engine::getJsonData<std::string>( data, "left_sdl_key_name" ) },
               .right_sdl_key_name { sdl_engine::getJsonData<std::string>( data, "right_sdl_key_name" ) }
            };
            registry.emplace<PlayerInput>( entities[ i ], input_comp );
         }
         // [BoundingBox]
         if ( entity_data.contains( "BoundingBox" ) )
         {
            auto&             data = entity_data.at( "BoundingBox" );
            myge::BoundingBox box_comp { .harf_width { sdl_engine::getJsonData<i32>( data, "harf_width" ) },
                                         .harf_hegiht { sdl_engine::getJsonData<i32>( data, "harf_height" ) },
                                         .radius { sdl_engine::getJsonData<f32>( data, "radius" ) },
                                         .state { BoundingBox::State::None },    // 固定
                                         .enable_axis { BoundingBox::EnableAxis::ALL } };
            // 有効判定方向に指定があれば再設定
            if ( data.contains( "enable_axis" ) )
            {
               auto axis { sdl_engine::getJsonData<std::string>( data, "enable_axis" ) };
               // allの場合すでに設定されてるので無視
               if ( axis != "all" )
               {
                  if ( axis == "top" ) { box_comp.enable_axis = BoundingBox::EnableAxis::Top; }
                  else if ( axis == "bottom" ) { box_comp.enable_axis = BoundingBox::EnableAxis::Bottom; }
                  else if ( axis == "left" ) { box_comp.enable_axis = BoundingBox::EnableAxis::Left; }
                  else if ( axis == "right" ) { box_comp.enable_axis = BoundingBox::EnableAxis::Right; }
                  else if ( axis == "lr" ) { box_comp.enable_axis = BoundingBox::EnableAxis::LR; }
                  else if ( axis == "tb" ) { box_comp.enable_axis = BoundingBox::EnableAxis::TB; }
               }
            }

            registry.emplace<myge::BoundingBox>( entities[ i ], box_comp );
         }
         // [OutOfScreenBehavior]
         if ( entity_data.contains( "OutOfScreenBehavior" ) )
         {
            auto&                     data = entity_data.at( "OutOfScreenBehavior" );
            myge::OutOfScreenBehavior behavior {};
            if ( data.at( "behavior" ) == "Destroy" ) { behavior.type = myge::OutOfScreenBehavior::Type::Destroy; }
            else if ( data.at( "behavior" ) == "Wrap" ) { behavior.type = myge::OutOfScreenBehavior::Type::Wrap; }
            else if ( data.at( "behavior" ) == "Stop" ) { behavior.type = myge::OutOfScreenBehavior::Type::Stop; }
            else if ( data.at( "behavior" ) == "Ignore" ) { behavior.type = myge::OutOfScreenBehavior::Type::Ignore; }
            registry.emplace<myge::OutOfScreenBehavior>( entities[ i ], behavior );
         }

         i++;
      }

      return entities;
   }
}    // namespace myge