#include <engine/Components/direct_control_tags.hpp>
#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/systems/movement_system.hpp>
namespace sdl_engine
{
   MovementSystem::MovementSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   MovementSystem::~MovementSystem() {}
   void MovementSystem::update( GameContext& context_ )
   {
      auto& registry   = context_.getRegistry();
      auto  delta_time = context_.getGameTimer().getDeltaTime();
      for ( auto [ entity, trfm, velo ] :
            registry.view<Transform, Velocity>( entt::exclude<DirectMovementTag> ).each() )
      {
         trfm.x += velo.dx * delta_time;
         trfm.y += velo.dy * delta_time;
      }
   }
}    // namespace sdl_engine
