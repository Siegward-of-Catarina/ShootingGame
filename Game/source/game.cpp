#include "game.hpp"

#include "scene.hpp"
#include "sequencer.hpp"
#include "test_scene.hpp"
namespace myge
{
   Game::Game() : _window { nullptr }, _render { nullptr }, _sequencer { nullptr } {}
   Game::~Game() {}
   bool Game::init()
   {
      if ( !SDL_Init( SDL_INIT_VIDEO ) )
      {
         SDL_Log( "SDLを初期化できませんでした:  %s ", SDL_GetError() );
         return false;
      }
      if ( !SDL_CreateWindowAndRenderer( "shooting", 400, 800, 0, &_window, &_render ) )
      {
         SDL_Log( "SDLwindow,SDLrendererを作成できませんでした: %s", SDL_GetError() );
         return false;
      }

      _sequencer = new Sequencer( new TestScene( this ) );
      return true;
   }
   void Game::run()
   {
      bool      quit = false;
      SDL_Event event;

      while ( !quit )
      {
         while ( SDL_PollEvent( &event ) )
         {
            if ( event.type == SDL_EVENT_QUIT )
            {
               quit = true;
               break;
            }
         }

         auto tick_time = SDL_GetTicks();
         auto delta     = ( tick_time - _last_time ) / 1000.f;
         _last_time     = tick_time;

         _sequencer->currentScene()->proc( delta );
      }
   }
   void Game::quit()
   {
      delete _sequencer;
      SDL_DestroyRenderer( _render );
      SDL_DestroyWindow( _window );
      SDL_Quit();
   }
}    // namespace myge