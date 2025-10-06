#include <SDL3/SDL.h>
#include <app/game.hpp>
#include <app/scene/title_scene.hpp>
#include <engine/basic_component.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/graphics.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/managers/system_manager.hpp>
namespace myge
{
   Game::Game() : _context { std::make_unique<sdl_engine::GameContext>( "shooting", 600, 800 ) }
   {
      _context->loadAssets( "assets/test_assets.json" );
      _context->getSceneManager().initStartCurrentScene( std::make_unique<TitleScene>( *_context ) );
   }

   Game::~Game() { SDL_Quit(); }

   void Game::run()
   {
      bool      quit = false;
      SDL_Event event;

      while ( !quit )
      {
         // msg loop
         while ( SDL_PollEvent( &event ) )
         {
            if ( event.type == SDL_EVENT_QUIT )
            {
               quit = true;
               break;
            }
            _context->getInputManager().handleEvent( event );
         }

         _context->update();
      }
   }
}    // namespace myge