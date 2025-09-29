#include <app/components/player_movement.hpp>
#include <app/systems/player_movement_system.hpp>
#include <engine/components/velocity.hpp>
#include <engine/core/game_context.hpp>
#include <engine/managers/input_manager.hpp>
namespace myge
{
   PlayerMovementSystem::PlayerMovementSystem() {}

   PlayerMovementSystem::~PlayerMovementSystem() {}

   void PlayerMovementSystem::update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ )
   {
      for ( auto [ entity, move, velo ] : registry_.view<PlayerAutoPilotMove, sdl_engine::Velocity>().each() ) {}
      for ( auto [ entity, move, velo ] : registry_.view<PlayerInputMove, sdl_engine::Velocity>().each() )
      {
         velo.dx              = 0;
         velo.dy              = 0;
         auto& input_manager_ = context_.getInputManager();
         if ( input_manager_.isKeyPress( move.up_code ) ) { velo.dy = -300.0f; }
         if ( input_manager_.isKeyPress( move.down_code ) ) { velo.dy = 300.0f; }
         if ( input_manager_.isKeyPress( move.left_code ) ) { velo.dx = -300.0f; }
         if ( input_manager_.isKeyPress( move.right_code ) ) { velo.dx = 300.0f; }
      }
   }

   int PlayerMovementSystem::priority() const { return 0; }
}    // namespace myge
