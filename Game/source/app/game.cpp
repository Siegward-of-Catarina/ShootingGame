#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/game.hpp>
#include <app/scene/title_scene.hpp>
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_system.hpp>
#include <engine/components/fade.hpp>
#include <engine/core.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/managers/system_manager.hpp>
#include <engine/systems/fade_system.hpp>

namespace myge
{
   Game::Game() : _context { std::make_unique<sdl_engine::EngineContext>( "shooting", 600, 800 ) }
   {
      _context->loadAssets( "assets/test_assets.json" );

      auto& resource_manager { _context->getResourceManager() };
      auto& registry { _context->getRegistry() };

      EntityFactory factory { registry, resource_manager };
      auto          window_size { _context->getWindowSize() };
      auto fade = factory.createDefaultFadeEntity( "OutIn", window_size.convertf32().x, window_size.convertf32().y );

      _context->getSystemManager().addSystem(
        std::make_unique<sdl_engine::FadeSystem>( 99, registry, _context->getEventListener() ) );

      _context->getSceneManager().enableFadeOutIn( fade );
      // 次のシーンとしてセットする
      sdl_engine::SceneDependencies dependencies { .registry { registry },
                                                   .dispatcher { _context->getDispatcher() },
                                                   .resource_manager { resource_manager },
                                                   .input_manager { _context->getInputManager() },
                                                   .scene_manager { _context->getSceneManager() },
                                                   .system_manager { _context->getSystemManager() } };

      _context->getSceneManager().initStartCurrentScene( std::make_unique<TitleScene>( dependencies ) );

      // アプリケーションの基幹システム
      _context->getSystemManager().addSystem(
        std::make_unique<InputSystem>( 0, registry, _context->getEventListener(), _context->getInputManager() ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::MovementSystem>( 96, registry ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::RotateSystem>( 97, registry ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::ScaleSystem>( 98, registry ) );
      _context->getSystemManager().addSystem(
        std::make_unique<LifeCycleSystem>( 100, registry, _context->getEventListener() ) );
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
         // エンジン側でquitが呼ばれた
         if ( _context->isQuit() ) { quit = true; }
      }
   }
}    // namespace myge