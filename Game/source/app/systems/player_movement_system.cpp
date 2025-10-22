#include <app/components/lifecycle_tags.hpp>
#include <app/components/player_input.hpp>
#include <app/components/player_movement.hpp>
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

   void PlayerMovementSystem::update( const sdl_engine::FrameData& frame_ )
   {
      for ( auto [ entity, input, velo, movement ] :
            registry().view<PlayerInput, sdl_engine::Velocity, PlayerMovementInput, ActiveTag>().each() )
      {
         velo.dx = 0;
         velo.dy = 0;
         input.move_direction.normalize();
         // 緩やかに加速・減速
         movement.smooth_dir.x +=
           ( movement.acceleration * frame_.delta_time ) * ( input.move_direction.x - movement.smooth_dir.x );
         movement.smooth_dir.y +=
           ( movement.acceleration * frame_.delta_time ) * ( input.move_direction.y - movement.smooth_dir.y );

         velo.dx = movement.smooth_dir.x * movement.max_speed;
         velo.dy = movement.smooth_dir.y * movement.max_speed;
      }
   }
}    // namespace myge
