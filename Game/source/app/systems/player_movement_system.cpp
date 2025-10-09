#include <app/components/player_movement.hpp>
#include <app/systems/player_movement_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{
   PlayerMovementSystem::PlayerMovementSystem( i32 priority_ ) : SystemInterface { priority_ } {}

   PlayerMovementSystem::~PlayerMovementSystem() {}

   void PlayerMovementSystem::update( sdl_engine::GameContext& context_ )
   {
      auto& registry = context_.getRegistry();
      for ( auto [ entity, move, velo ] : registry.view<PlayerAutoPilotMove, sdl_engine::Velocity>().each() ) {}
      for ( auto [ entity, move, velo ] : registry.view<PlayerInputMove, sdl_engine::Velocity>().each() )
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
}    // namespace myge
