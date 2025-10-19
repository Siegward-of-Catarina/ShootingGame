#include <app/components/affiliation.hpp>
#include <app/components/bounding_box.hpp>
#include <app/components/button_ui.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/highlightable.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/components/shooter.hpp>
#include <app/components/sprite_brink.hpp>
#include <app/components/title_input.hpp>
#include <app/components/title_menu.hpp>
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
      sdl_engine::Fade fade_comp { .state { sdl_engine::Fade::State::Idle },
                                   .speed { 2.0f },
                                   .black_out_duration { 1.0f } };
      _registry.emplace<sdl_engine::Fade>( fade, fade_comp );
      _registry.emplace<sdl_engine::RenderFadeTag>( fade );
      return fade;
   }
   entt::entity EntityFactory::createBullet( entt::entity& shooter_, const std::type_index& affiliation_id_ )
   {
      using namespace sdl_engine;

      auto bullet_entt { _registry.create() };

      // affiliation
      setAffiliationTag( bullet_entt, affiliation_id_ );

      _registry.emplace<EnteringTag>( bullet_entt );
      _registry.emplace<sdl_engine::RenderableTag>( bullet_entt );
      _registry.emplace<sdl_engine::UpdateableTag>( bullet_entt );
      _registry.emplace<PlayerBulletTag>( bullet_entt );
      _registry.emplace<RenderGameSpriteTag>( bullet_entt );

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

      return bullet_entt;
   }
   entt::entity EntityFactory::createPlayer( json& data_, const std::type_index& affiliation_id_ )
   {

      auto entity { _registry.create() };

      setAffiliationTag( entity, affiliation_id_ );

      // リソース
      auto sprt_resource { _resource_manager.getSprite( "player" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "player_anim" ) };

      // タグコンポーネント
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
      _registry.emplace<PlayerTag>( entity );
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      // Sprite
      sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
      sprt.dst.w = static_cast<f32>( sprt_anim_resource->frame_width );
      sprt.dst.h = static_cast<f32>( sprt_anim_resource->frame_height );
      _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      // SpriteAnim
      sdl_engine::SpriteAnim sp_anim { sdl_engine::createSpriteAnim( sprt_anim_resource, 1u ) };
      _registry.emplace<sdl_engine::SpriteAnim>( entity, sp_anim );
      // Transform
      sdl_engine::Transform trfm { 0.0f, 0.0f, 0.0f, 1.0f };
      if ( auto pos { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "pos" ) }; pos )
      {
         // array生成に対応するためoffsetを足す
         trfm.x = pos.value()[ 0 ];
         trfm.y = pos.value()[ 1 ];
      }
      _registry.emplace<sdl_engine::Transform>( entity, trfm );
      // Velocity
      sdl_engine::Velocity velo { 0.0f, 10.0f, 0.0f, 0.0f };
      if ( auto dir { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "dir" ) }; dir )
      {
         velo.dx = dir.value()[ 0 ];
         velo.dy = dir.value()[ 1 ];
      }
      _registry.emplace<sdl_engine::Velocity>( entity, velo );
      // BBox
      BoundingBox box { createBoundingBox( sprt.dst.w / 2, sprt.dst.h / 2, 20.0f ) };
      _registry.emplace<BoundingBox>( entity, box );
      // Input
      _registry.emplace<PlayerInput>( entity );
      // Shooter
      Shooter shtr { createShooter( 0.2f, sdl_engine::Vector2_f32 { 0.0f, -500.0f }, "enemy_small" ) };

      _registry.emplace<Shooter>( entity, shtr );
      return entity;
   }
   entt::entity EntityFactory::createWandererEnemy( json&                   data_,
                                                    const std::type_index&  affiliation_id_,
                                                    sdl_engine::Vector2_f32 offset_pos_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      auto sprt_resource { _resource_manager.getSprite( "enemy1" ) };
      auto sprt_anim_resource { _resource_manager.getSpriteAnim( "enemy1_anim" ) };
      // タグ
      _registry.emplace<sdl_engine::RenderGameSpriteTag>( entity );
      _registry.emplace<EnemyTag>( entity );
      if ( auto interval { sdl_engine::getJsonData<f32>( data_, "interval" ) }; interval )
      {
         _registry.emplace<WaitTag>( entity, interval.value(), 0.0f );
      }
      else
      {
         _registry.emplace<EnteringTag>( entity );
         _registry.emplace<sdl_engine::RenderableTag>( entity );
         _registry.emplace<sdl_engine::UpdateableTag>( entity );
      }

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
   entt::entity EntityFactory::createBasicUI( json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      if ( auto sp_name { sdl_engine::getJsonData<std::string>( data_, "sprite_name" ) }; sp_name )
      {
         sprt_resource = _resource_manager.getSprite( sp_name.value() );
      }

      // タグコンポーネント
      _registry.emplace<sdl_engine::RenderUITag>( entity );
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      // Sprite
      sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
      _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      // Transform
      sdl_engine::Transform trfm { 0.0f, 0.0f, 0.0f, 1.0f };
      if ( auto pos { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "pos" ) }; pos )
      {
         // array生成に対応するためoffsetを足す
         trfm.x = pos.value()[ 0 ];
         trfm.y = pos.value()[ 1 ];
      }
      _registry.emplace<sdl_engine::Transform>( entity, trfm );
      return entity;
   }
   entt::entity EntityFactory::createHighlightableUI( json& data_, const std::type_index& affiliation_id_ )
   {
      auto          entity { createBasicUI( data_, affiliation_id_ ) };    // selectable
      auto          sprt { _registry.get<sdl_engine::Sprite>( entity ) };
      auto          inactive_color { sprt.color * 0.5 };
      Highlightable selectable { false, sprt.color, inactive_color };
      _registry.emplace<Highlightable>( entity, selectable );
      return entity;
   }
   entt::entity EntityFactory::createBasicText( json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      entt::resource<sdl_engine::FontResource>   font_resource {};
      if ( auto font_size { sdl_engine::getJsonData<std::string>( data_, "font_size" ) }; font_size )
      {
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
      }
      // タグコンポーネント
      _registry.emplace<myge::EnteringTag>( entity );
      _registry.emplace<sdl_engine::RenderableTag>( entity );
      _registry.emplace<sdl_engine::UpdateableTag>( entity );
      _registry.emplace<sdl_engine::RenderTextTag>( entity );
      // Sprite
      sdl_engine::Sprite sprt { sdl_engine::createSprite( sprt_resource ) };
      auto               color { data_.value( "color", std::array<f32, 4> { 1.0f, 1.0f, 1.0f, 1.0f } ) };
      sprt.color = sdl_engine::ColorRGBA( color );
      _registry.emplace<sdl_engine::Sprite>( entity, sprt );
      // Transform
      sdl_engine::Transform trfm { 0.0f, 0.0f, 0.0f, 1.0f };
      if ( auto pos { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "pos" ) }; pos )
      {
         // array生成に対応するためoffsetを足す
         trfm.x = pos.value()[ 0 ];
         trfm.y = pos.value()[ 1 ];
      }
      _registry.emplace<sdl_engine::Transform>( entity, trfm );
      // Text
      sdl_engine::Text text { font_resource, "none" };
      if ( auto text_data { sdl_engine::getJsonData<std::string>( data_, "text" ) }; text_data )
      {
         text.text = text_data.value();
      }
      _registry.emplace<sdl_engine::Text>( entity, text );

      return entity;
   }
   entt::entity EntityFactory::createBrinkText( json& data_, const std::type_index& affiliation_id_ )
   {
      auto entity { createBasicText( data_, affiliation_id_ ) };

      // Brink
      SpriteBrink brink { data_.value( "brink_speed", 1.0f ), data_.value( "brink_min_alpha", 0.3f ) };
      _registry.emplace<SpriteBrink>( entity, brink );

      return entity;
   }
   entt::entity EntityFactory::createHighlightableText( json& data_, const std::type_index& affiliation_id_ )
   {

      auto entity { createBasicText( data_, affiliation_id_ ) };
      // selectable
      auto          sprt { _registry.get<sdl_engine::Sprite>( entity ) };
      auto          inactive_color { sprt.color * 0.5 };
      Highlightable selectable { false, sprt.color, inactive_color };
      if ( auto active_data { sdl_engine::getJsonData<u32>( data_, "highlightable" ) }; active_data )
      {
         selectable.active = active_data.value();
      }
      _registry.emplace<Highlightable>( entity, selectable );
      return entity;
   }
   entt::entity EntityFactory::createTitleMenu( json& data_, const std::type_index& affiliation_id_ )
   {
      entt::entity entity { _registry.create() };
      setAffiliationTag( entity, affiliation_id_ );
      TitleMenu menu { .menu_ui {}, .selected { 0 } };
      auto      highlight_ui { sdl_engine::getJsonData<json>( data_, "highlightable_ui" ).value() };
      for ( auto& ui_data : highlight_ui )
      {
         auto     ui_entt { createHighlightableUI( ui_data, affiliation_id_ ) };
         ButtonUI button_ui {};
         auto     button { sdl_engine::getJsonData<std::string>( ui_data, "button_type" ).value() };
         if ( button == "start" ) { button_ui.type = ButtonUI::Type::Start; }
         else if ( button == "exit" ) { button_ui.type = ButtonUI::Type::Exit; }
         _registry.emplace<ButtonUI>( ui_entt, button_ui );
         menu.menu_ui.emplace_back( ui_entt );
      }
      _registry.emplace<TitleMenu>( entity, menu );
      _registry.emplace<TitleInput>( entity );
      return entity;
   }
   std::pair<entt::entity, entt::entity> EntityFactory::createBackGround( json&                  data_,
                                                                          const std::type_index& affiliation_id_ )
   {

      std::array<entt::entity, 2> entities { _registry.create(), _registry.create() };
      setAffiliationTag( entities[ 0 ], affiliation_id_ );
      setAffiliationTag( entities[ 1 ], affiliation_id_ );
      // リソース
      entt::resource<sdl_engine::SpriteResource> sprt_resource {};
      if ( auto sp_name { sdl_engine::getJsonData<std::string>( data_, "sprite_name" ) }; sp_name )
      {
         sprt_resource = _resource_manager.getSprite( sp_name.value() );
      }
      else { sprt_resource = _resource_manager.getSprite( "back_ground" ); }

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
         sprt.render_order = data_.value( "z_order", 100 );
         _registry.emplace<sdl_engine::Sprite>( entt, sprt );
         // Transform
         sdl_engine::Transform trfm { 300.0f, 0.0f, 0.0f, 1.0f };
         if ( auto pos { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "pos" ) }; pos )
         {
            // array生成に対応するためoffsetを足す
            trfm.x = pos.value()[ 0 ];
            trfm.y = pos.value()[ 1 ] - sprt.dst.h * i++;
         }
         _registry.emplace<sdl_engine::Transform>( entt, trfm );
         // Velocity
         sdl_engine::Velocity velo { 0.0f, 10.0f, 0.0f, 0.0f };
         if ( auto dir { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "dir" ) }; dir )
         {
            velo.dx = dir.value()[ 0 ];
            velo.dy = dir.value()[ 1 ];
         }
         _registry.emplace<sdl_engine::Velocity>( entt, velo );
         // BBox
         BoundingBox box { createBoundingBox( sprt.dst.w / 2, sprt.dst.h / 2, 0.0f, BoundingBox::EnableAxis::Top ) };
         _registry.emplace<BoundingBox>( entt, box );
      }

      return std::pair { entities[ 0 ], entities[ 1 ] };
   }
   std::vector<entt::entity> EntityFactory::createWandererEnemyArray( json&                  data_,
                                                                      const std::type_index& affiliation_id_ )
   {
      auto                      num_enemy { sdl_engine::getJsonData<u32>( data_, "num" ) };
      auto                      offset { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "offset_pos" ) };
      std::vector<entt::entity> entities { num_enemy.value() };

      for ( u32 i = 0; i < num_enemy.value(); i++ )
      {
         sdl_engine::Vector2_f32 vec;
         vec.x         = offset.value()[ 0 ] * i;
         vec.y         = offset.value()[ 1 ] * i;
         entities[ i ] = createWandererEnemy( data_, affiliation_id_, vec );
      }
      return entities;
   }
   std::vector<entt::entity> EntityFactory::createEntities( json&                           data_,
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