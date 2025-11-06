#include <pch.hpp>
// my header
#include <engine/systems/scale_system.hpp>
// component
#include <engine/components/Velocity.hpp>
#include <engine/components/direct_control_tags.hpp>
#include <engine/components/transform.hpp>
// core
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
namespace sdl_engine
{
   ScaleSystem::ScaleSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ } {}
   ScaleSystem::~ScaleSystem() {}
   void ScaleSystem::update( const FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, trfm, velo ] : reg.view<Transform, Velocity>( entt::exclude<DirectScaleTag> ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         trfm.scale += velo.scale_rate * frame_.delta_time;
      }
   }
}    // namespace sdl_engine