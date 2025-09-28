#include <app/systems/move_system.hpp>
#include <engine/Components/velocity.hpp>
#include <engine/components/transform.hpp>
namespace myge
{
   MoveSystem::MoveSystem() {}
   MoveSystem::~MoveSystem() {}
   void MoveSystem::update( entt::registry& registry_, f32 delta_time )
   {
      for ( auto [ entity, trfm, velo ] : registry_.view<sdl_engine::Transform, sdl_engine::Velocity>().each() )
      {
         trfm.x += velo.dx * delta_time;
         trfm.y += velo.dy * delta_time;
         if ( trfm.y > 800.0f ) { trfm.y = -799.0f; }
      }
   }
}    // namespace myge
