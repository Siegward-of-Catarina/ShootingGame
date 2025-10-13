#include <engine/Components/direct_control_tags.hpp>
#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/systems/scale_system.hpp>
namespace sdl_engine
{
   ScaleSystem::ScaleSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ } {}
   ScaleSystem::~ScaleSystem() {}
   void ScaleSystem::update( EngineContext& context_ )
   {

      auto& registry { getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };

      for ( auto [ entity, trfm, velo ] : registry.view<Transform, Velocity>( entt::exclude<DirectScaleTag> ).each() )
      {
         trfm.scale += velo.scale_rate * delta_time;
      }
   }
}    // namespace sdl_engine