#include <pch.hpp>
// my header
#include <app/game.hpp>
// scene
#include <app/scene/title_scene.hpp>
// factory
#include <app/entity_factory.hpp>
// system
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_system.hpp>
#include <engine/systems/fade_system.hpp>
// component
#include <engine/components/fade.hpp>
// core
#include <engine/core.hpp>
// manager
#include <engine/managers/input_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/managers/system_manager.hpp>
namespace myge
{
   Game::Game() : _context { nullptr }
   {
      // エンジンコンテキスト初期化
      sdl_engine::EngineInitInfo init_info {
         .window_name { "shooting" }, .window_width { 600 }, .window_height { 800 }, .se_track_size { 32 }
      };
      _context = std::make_unique<sdl_engine::EngineContext>( init_info );
      // アセット読み込み
      _context->loadAssets( "assets/assets.json" );

      auto& resource_manager { _context->getResourceManager() };
      auto& registry { _context->getRegistry() };

      EntityFactory factory { registry, resource_manager };
      auto          window_size { _context->getWindowSize() };
      auto fade = factory.createDefaultFadeEntity( "OutIn", window_size.convertf32().x, window_size.convertf32().y );

      _context->getSystemManager().addSystem(
        std::make_unique<sdl_engine::FadeSystem>( 99, registry, _context->getEventListener() ) );

      _context->getSceneManager().enableFadeOutIn( fade );

      // シーンセット
      auto depend { _context->getSceneDependencies() };
      _context->getSceneManager().initStartCurrentScene( std::make_unique<TitleScene>( depend ) );

      // アプリケーションの基幹システム
      _context->getSystemManager().addSystem(
        std::make_unique<InputSystem>( 0, registry, _context->getEventListener(), _context->getInputManager() ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::MovementSystem>( 96, registry ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::RotateSystem>( 97, registry ) );
      _context->getSystemManager().addSystem( std::make_unique<sdl_engine::ScaleSystem>( 98, registry ) );
      _context->getSystemManager().addSystem(
        std::make_unique<LifeCycleSystem>( 100, registry, _context->getEventListener() ) );
   }

   Game::~Game() {}

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