#include <engine/Components/direct_control_tags.hpp>
#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/systems/rotate_system.hpp>
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