#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/systems/move_system.hpp>
#include <app/waves/wave1.hpp>
#include <engine/components/sprite.hpp>
#include <engine/components/sprite_anim.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/rendering/loader/sprite_anim_resource.hpp>
#include <engine/rendering/loader/sprite_resource.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace
{
   float    frame { 0 };
   uint32_t max_frame { 8 };

}    // namespace
namespace myge
{
   Wave1::Wave1() : Wave {}, _move_system {} {}

   void Wave1::start( entt::registry& registry_, sdl_engine::GameContext& context_ )
   {
      createEnemys( registry_, context_ );

      // 描画用にエンティティをソート
      registry_.sort<sdl_engine::Sprite>( []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
                                          { return l_sprt.texture->depth > r_sprt.texture->depth; } );

      for ( auto [ entity, sprt ] : registry_.view<sdl_engine::Sprite>().each() )
      {
         SDL_Log( "%d", sprt.texture->depth );
      }
   }
   void Wave1::update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ )
   {
      _move_system->update( registry_, delta_time_ );
      _enemy_system->update( registry_, delta_time_ );
      for ( auto [ entity, trfm, sprt ] : registry_.view<sdl_engine::Transform, sdl_engine::Sprite>().each() )
      {
         sprt.dst.x = trfm.x;
         sprt.dst.y = trfm.y;
      }

      for ( auto [ entity, sprt, anim ] : registry_.view<sdl_engine::Sprite, sdl_engine::SpriteAnim>().each() )
      {
         anim.current_frame += delta_time_ * anim.sprite_anim->frame_time;
         auto idx = static_cast<u32>( anim.current_frame ) % anim.sprite_anim->frame_num;

         sprt.src   = anim.sprite_anim->frames[ idx ];
         sprt.dst.w = static_cast<f32>( anim.sprite_anim->frame_width );
         sprt.dst.h = static_cast<f32>( anim.sprite_anim->frame_height );
      }
   }
   void Wave1::createEnemys( entt::registry& registry_, sdl_engine::GameContext& context_ )
   {
      EntityFactory factory;
      if ( _wave_data.contains( "Enemy1" ) )
      {
         auto enemy_datas      = _wave_data.at( "Enemy1" );
         auto sprite_name      = sdl_engine::getJsonData<std::string>( enemy_datas, "sprite_name" );
         auto sprite_anim_name = sdl_engine::getJsonData<std::string>( enemy_datas, "sprite_anim_name" );

         for ( auto& enemy_data : enemy_datas.at( "Datas" ) )
         {
            enemy_data[ "sprite_name" ]      = sprite_name;
            enemy_data[ "sprite_anim_name" ] = sprite_anim_name;
            auto enemy = factory.createEnemy( registry_, context_.getResourceManager(), enemy_data );
         }
      }
   }
}    // namespace myge
