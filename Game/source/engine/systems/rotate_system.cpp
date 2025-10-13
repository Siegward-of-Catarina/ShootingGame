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
   void RotateSystem::update( EngineContext& context_ )
   {
      auto& registry { getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };

      for ( auto [ entity, trfm, velo ] : registry.view<Transform, Velocity>( entt::exclude<DirectRotateTag> ).each() )
      {
         trfm.angle += velo.anguler * delta_time;
      }
   }
}    // namespace sdl_engine