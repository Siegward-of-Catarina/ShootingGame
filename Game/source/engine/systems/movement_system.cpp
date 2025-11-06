#include <pch.hpp>
// my header
#include <engine/systems/movement_system.hpp>
// component
#include <engine/components/direct_control_tags.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/velocity.hpp>
// core
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
namespace sdl_engine
{
   MovementSystem::MovementSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ }
   {
   }
   MovementSystem::~MovementSystem() {}
   void MovementSystem::update( const FrameData& frame_ )
   {
      auto& reg { registry() };
      auto  view { getUpdateable<Transform, Velocity>( reg, entt::exclude<DirectMovementTag> ) };
      for ( auto [ entity, trfm, velo ] : view.each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         trfm.position.x += velo.vector.x * frame_.delta_time;
         trfm.position.y += velo.vector.y * frame_.delta_time;
      }
   }
}    // namespace sdl_engine
