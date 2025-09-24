#include <SDL3/SDL.h>
#include <core/core.hpp>
#include <game.hpp>
#include <renderer/renderer.hpp>
#include <resource/resource_manager.hpp>
#include <scene/scene.hpp>
#include <scene/test_scene.hpp>
#include <sequencer.hpp>
namespace myge
{
   Game::Game()
     : _window { nullptr, nullptr }
     , _renderer { nullptr }
     , _sequencer { nullptr }
     , _resource_manager { nullptr }
     , _last_time { 0 }
   {
      // あさーとを使うか例外を出すか
      if ( !SDL_Init( SDL_INIT_VIDEO ) ) { throw GameException( "SDLの初期化に失敗しました" ); }

      // window, renderer 取得
      // いったん生ポインタで取得し、unique_ptrへ
      SDL_Window* window_raw = SDL_CreateWindow( "shooting", 640, 800, 0 );
      if ( !window_raw ) { throw GameException( "SDLwindowを作成できませんでした" ); }

      // uniqueへ代入
      _window = { window_raw, &SDL_DestroyWindow };

      _renderer = std::make_unique<Renderer>( window_raw );

      _sequencer = std::make_unique<Sequencer>();

      _resource_manager = std::make_unique<ResourceManager>();

      _sequencer->setNextScene(
        std::make_unique<TestScene>( SceneInitDesc( *_renderer, *_sequencer, *_resource_manager ) ) );

      _last_time = SDL_GetPerformanceCounter();
   }

   Game::~Game() { SDL_Quit(); }

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
         // NSで取得し、Secへ変換
         u64  tick_time = SDL_GetPerformanceCounter();
         auto delta     = static_cast<double>( tick_time - _last_time ) / SDL_GetPerformanceFrequency();
         _last_time     = tick_time;

         _sequencer->currentScene().proc( delta );
      }
   }
}    // namespace myge