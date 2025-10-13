#include <SDL3/SDL.h>
#include <app/game.hpp>
#include <app/scene/title_scene.hpp>
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_system.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/managers/system_manager.hpp>
namespace myge
{
   Game::Game() : _context { std::make_unique<sdl_engine::EngineContext>( "shooting", 600, 800 ) }
   {
      _context->loadAssets( "assets/test_assets.json" );

      sdl_engine::SceneDependencies dependencies { .registry { _context->getRegistry() },
                                                   .resource_manager { _context->getResourceManager() },
                                                   .scene_manager { _context->getSceneManager() },
                                                   .system_manager { _context->getSystemManager() } };

      _context->getSceneManager().initStartCurrentScene( std::make_unique<TitleScene>( dependencies ) );

      // アプリケーションの基幹システム
      _context->getSystemManager().addSystem( std::make_unique<LifeCycleSystem>( 99, _context->getRegistry() ) );
      // ベーシックなシステムを追加

      _context->getSystemManager().addSystem(
        std::make_unique<InputSystem>( 0, _context->getRegistry(), _context->getInputManager() ) );
      _context->getSystemManager().addSystem(
        std::make_unique<sdl_engine::MovementSystem>( 96, _context->getRegistry() ) );
      _context->getSystemManager().addSystem(
        std::make_unique<sdl_engine::RotateSystem>( 97, _context->getRegistry() ) );
      _context->getSystemManager().addSystem(
        std::make_unique<sdl_engine::ScaleSystem>( 98, _context->getRegistry() ) );
      // アニメーションは必要ない場面もあるかもしれないが、
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