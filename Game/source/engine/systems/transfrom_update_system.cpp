#include <engine/Components/active.hpp>
#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/systems/transfrom_update_system.hpp>
namespace sdl_engine
{
   TransformUpdateSystem::TransformUpdateSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   TransformUpdateSystem::~TransformUpdateSystem() {}
   void TransformUpdateSystem::update( GameContext& context_ )
   {
      auto& registry   = context_.getRegistry();
      auto  delta_time = context_.getGameTimer().getDeltaTime();
      for ( auto [ entity, velo ] : registry.view<Velocity, Active>().each() )
      {
         auto& trfm = registry.get<Transform>( entity );
         trfm.x += velo.dx * delta_time;
         trfm.y += velo.dy * delta_time;
         trfm.angle += velo.anguler * delta_time;
         trfm.scale += velo.scale_rate * delta_time;
      }
   }
}    // namespace sdl_engine
