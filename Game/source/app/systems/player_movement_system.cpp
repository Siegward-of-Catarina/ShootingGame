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
         velo.vector.x = 0;
         velo.vector.y = 0;
         input.move_direction.normalize();
         // 緩やかに加速・減速
         if ( input.move_direction.x != 0 )
         {
            movement.smooth_dir.x +=
              ( movement.acceleration * frame_.delta_time ) * ( input.move_direction.x - movement.smooth_dir.x );
         }
         else { movement.smooth_dir.x -= movement.smooth_dir.x * ( movement.deceleration * frame_.delta_time ); }

         if ( input.move_direction.y != 0 )
         {
            movement.smooth_dir.y +=
              ( movement.acceleration * frame_.delta_time ) * ( input.move_direction.y - movement.smooth_dir.y );
         }
         else { movement.smooth_dir.y -= movement.smooth_dir.y * ( movement.deceleration * frame_.delta_time ); }

         velo.vector.x = movement.smooth_dir.x * movement.max_speed;
         velo.vector.y = movement.smooth_dir.y * movement.max_speed;
      }
   }
}    // namespace myge
