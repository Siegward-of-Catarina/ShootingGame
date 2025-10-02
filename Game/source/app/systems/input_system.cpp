#include <app/components/player_input.hpp>
#include <app/systems/input_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>

namespace myge
{

   InputSystem::InputSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   InputSystem::~InputSystem() {}
   void InputSystem::update( sdl_engine::GameContext& context_ )
   {
      auto& registry = context_.getRegistry();
      for ( auto [ entity, input, velo, anim ] :
            registry.view<PlayerInput, sdl_engine::Velocity, sdl_engine::SpriteAnim>().each() )
      {
         velo.dx              = 0;
         velo.dy              = 0;
         anim.current_frame   = 1;
         auto& input_manager_ = context_.getInputManager();
         if ( input_manager_.isKeyPress( input.up_sdl_key_name ) ) { velo.dy = -300.0f; }
         if ( input_manager_.isKeyPress( input.down_sdl_key_name ) ) { velo.dy = 300.0f; }
         if ( input_manager_.isKeyPress( input.left_sdl_key_name ) )
         {
            velo.dx            = -300.0f;
            anim.current_frame = anim.sprite_anim->frame_num - 1;
         }
         if ( input_manager_.isKeyPress( input.right_sdl_key_name ) )
         {
            velo.dx            = 300.0f;
            anim.current_frame = 0;
         }
      }
   }

}    // namespace myge
