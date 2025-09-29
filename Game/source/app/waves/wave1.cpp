#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
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
      if ( _wave_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         factory.createEntities( registry_, context_.getResourceManager(), _wave_data.at( "Entities" ) );
      }

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
      _enemy_system->update( registry_, context_, delta_time_ );

      _active_time -= delta_time_;
   }
}    // namespace myge
