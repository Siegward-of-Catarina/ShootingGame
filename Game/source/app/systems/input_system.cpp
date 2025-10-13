#include <app/components/player_input.hpp>
#include <app/components/title_input.hpp>
#include <app/systems/input_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>

namespace myge
{

   InputSystem::InputSystem( i32 priority_, entt::registry& registry_, sdl_engine::InputManager& input_manager_ )
     : SystemInterface { priority_, registry_ }, _input_manager { input_manager_ }
   {
   }
   InputSystem::~InputSystem() {}
   void InputSystem::update( sdl_engine::EngineContext& context_ )
   {
      auto& registry { getRegistry() };
      for ( auto [ entity, input, anim ] : getLogicUpdateable<PlayerInput, sdl_engine::SpriteAnim>( registry ).each() )
      {
         anim.current_frame   = 1;
         input.move_direction = { 0.0f, 0.0f };
         input.isShoot        = false;
         if ( _input_manager.isKeyPress( SDL_SCANCODE_UP ) ) { input.move_direction.y = -1.0f; }
         if ( _input_manager.isKeyPress( SDL_SCANCODE_DOWN ) ) { input.move_direction.y = 1.0f; }
         if ( _input_manager.isKeyPress( SDL_SCANCODE_LEFT ) )
         {
            input.move_direction.x = -1.0f;
            anim.current_frame     = anim.sprite_anim->frame_num - 1;
         }
         if ( _input_manager.isKeyPress( SDL_SCANCODE_RIGHT ) )
         {
            input.move_direction.x = 1.0f;
            anim.current_frame     = 0;
         }
         if ( _input_manager.isKeyPress( SDL_SCANCODE_Z ) ) { input.isShoot = true; };
      }

      for ( auto [ entity, input ] : registry.view<TitleInput>().each() )
      {
         if ( _input_manager.isAnyKeydown() ) { input.any_key = true; }
      }
   }

}    // namespace myge
