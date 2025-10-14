#include <app/components/player_input.hpp>
#include <app/systems/player_movement_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{
   PlayerMovementSystem::PlayerMovementSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }

   PlayerMovementSystem::~PlayerMovementSystem() {}

   void PlayerMovementSystem::update(const sdl_engine::FrameData& frame_)
   {
      for ( auto [ entity, input, velo ] : registry().view<PlayerInput, sdl_engine::Velocity>().each() )
      {
         velo.dx = 0;
         velo.dy = 0;
         input.move_direction.normalize();
         velo.dx = input.move_direction.x * 300.0f;
         velo.dy = input.move_direction.y * 300.0f;
      }
   }
}    // namespace myge
