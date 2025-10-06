#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/waves/wave1.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
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

   void Wave1::start( sdl_engine::GameContext& context_ )
   {
      std::vector<entt::entity> entities;
      if ( _wave_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         entities = factory.createEntities( context_, _wave_data.at( "Entities" ) );
      }
      auto& registry = context_.getRegistry();

      for ( auto [ entity, sprt ] : registry.view<sdl_engine::Sprite>().each() )
      {
         SDL_Log( "%d", sprt.texture->depth );
      }
   }
   void Wave1::update( sdl_engine::GameContext& context_ ) { _active_time -= context_.getGameTimer().getDeltaTime(); }
}    // namespace myge
