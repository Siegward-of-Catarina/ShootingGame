#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/waves/wave1.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/rendering/resource/sprite_anim_resource.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace
{
   float                     frame { 0 };
   uint32_t                  max_frame { 8 };
   std::vector<entt::entity> entities;
}    // namespace
namespace myge
{
   Wave1::Wave1( WaveDependencies& dependencies_ ) : Wave { dependencies_ } {}

   void Wave1::start()
   {
      auto& wave_data { getWaveData() };
      if ( wave_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         entities = factory.createEntities( registry(), resourceManager(), wave_data.at( "Entities" ) );
      }
      for ( auto [ entity, sprt ] : registry().view<sdl_engine::Sprite>().each() )
      {
         SDL_Log( "%d", sprt.render_order );
      }
   }
   void Wave1::update( f32 delta_time_ )
   {
      i32 i { 0 };
      for ( auto& entity : entities )
      {
         if ( !registry().valid( entity ) ) { i++; }
         else { break; }
      }

      if ( i == entities.size() ) { endWave(); }
   }
}    // namespace myge
