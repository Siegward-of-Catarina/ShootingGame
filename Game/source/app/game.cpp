#include <SDL3/SDL.h>
#include <app/game.hpp>
#include <app/scene/test_scene.hpp>
#include <engine/core/game_context.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/scene_manager.hpp>
namespace myge
{
   Game::Game() : _context { std::make_unique<sdl_engine::GameContext>( "shooting", 600, 800 ) }, _last_time { 0 }
   {

      _context->getSequencer().setCurrentScene( new TestScene( *_context ) );
      _last_time = SDL_GetPerformanceCounter();
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

         // NSで取得し、Secへ変換
         u64  tick_time = SDL_GetPerformanceCounter();
         auto delta     = static_cast<f32>( tick_time - _last_time ) / SDL_GetPerformanceFrequency();
         _last_time     = tick_time;

         _context->getSequencer().currentScene().proc( delta );
         _context->getInputManager().update();
      }
   }
}    // namespace myge