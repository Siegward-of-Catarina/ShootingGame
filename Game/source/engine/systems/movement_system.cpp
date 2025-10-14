#include <engine/Components/direct_control_tags.hpp>
#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/systems/movement_system.hpp>
namespace sdl_engine
{
   MovementSystem::MovementSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ }
   {
   }
   MovementSystem::~MovementSystem() {}
   void MovementSystem::update(const FrameData& frame_)
   {
      for ( auto [ entity, trfm, velo ] :
            registry().view<Transform, Velocity>( entt::exclude<DirectMovementTag> ).each() )
      {
         trfm.x += velo.dx * frame_.delta_time;
         trfm.y += velo.dy * frame_.delta_time;
      }
   }
}    // namespace sdl_engine
