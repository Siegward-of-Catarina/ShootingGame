#include <app/components/player_input.hpp>
#include <app/components/title_input.hpp>
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
      auto& registry      = context_.getRegistry();
      auto& input_manager = context_.getInputManager();
      for ( auto [ entity, input, anim ] : getLogicUpdateable<PlayerInput, sdl_engine::SpriteAnim>( registry ).each() )
      {
         anim.current_frame   = 1;
         input.move_direction = { 0.0f, 0.0f };
         input.isShoot        = false;
         if ( input_manager.isKeyPress( SDL_SCANCODE_UP ) ) { input.move_direction.y = -1.0f; }
         if ( input_manager.isKeyPress( SDL_SCANCODE_DOWN ) ) { input.move_direction.y = 1.0f; }
         if ( input_manager.isKeyPress( SDL_SCANCODE_LEFT ) )
         {
            input.move_direction.x = -1.0f;
            anim.current_frame     = anim.sprite_anim->frame_num - 1;
         }
         if ( input_manager.isKeyPress( SDL_SCANCODE_RIGHT ) )
         {
            input.move_direction.x = 1.0f;
            anim.current_frame     = 0;
         }
         if ( input_manager.isKeyPress( SDL_SCANCODE_Z ) ) { input.isShoot = true; };
      }

      for ( auto [ entity, input ] : registry.view<TitleInput>().each() )
      {
         if ( input_manager.isAnyKeydown() ) { input.any_key = true; }
      }
   }

}    // namespace myge
