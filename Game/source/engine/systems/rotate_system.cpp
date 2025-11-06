#include <pch.hpp>
// my header
#include <engine/systems/rotate_system.hpp>
// component
#include <engine/components/direct_control_tags.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/velocity.hpp>
// core
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
namespace sdl_engine
{
   RotateSystem::RotateSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ } {}
   RotateSystem::~RotateSystem() {}
   void RotateSystem::update( const FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, trfm, velo ] : reg.view<Transform, Velocity>( entt::exclude<DirectRotateTag> ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         trfm.angle += velo.anguler * frame_.delta_time;
      }
   }
}    // namespace sdl_engine