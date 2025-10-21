#include <app/components/player_input.hpp>
#include <app/systems/player_movement_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace
{
   constexpr f32           max_speed    = 400.0f;
   f32                     acceleration = 5.0f;
   sdl_engine::Vector2_f32 dir { 0.0f, 0.0f };
}    // namespace
namespace myge
{
   PlayerMovementSystem::PlayerMovementSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }

   PlayerMovementSystem::~PlayerMovementSystem() {}

   void PlayerMovementSystem::update( const sdl_engine::FrameData& frame_ )
   {
      for ( auto [ entity, input, velo ] : registry().view<PlayerInput, sdl_engine::Velocity>().each() )
      {
         velo.dx = 0;
         velo.dy = 0;
         input.move_direction.normalize();
         dir.x   = dir.x + ( acceleration * frame_.delta_time ) * ( input.move_direction.x - dir.x );
         dir.y   = dir.y + ( acceleration * frame_.delta_time ) * ( input.move_direction.y - dir.y );
         velo.dx = dir.x * max_speed;
         velo.dy = dir.y * max_speed;
      }
   }
}    // namespace myge
