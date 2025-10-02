#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/systems/input_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
// engine
#include <engine/core.hpp>

namespace myge
{
   TestScene::TestScene( sdl_engine::GameContext& ctx_ ) : Scene { ctx_ }, _scene_elapsed_time { 0.f }
   {
      //_player_movement_system = std::make_unique<PlayerMovementSystem>();
      auto& system_manager { getGameContext().getSystemManager() };
      system_manager.addSystem( typeid( InputSystem ), std::make_unique<InputSystem>( 0 ) );
      system_manager.addSystem( typeid( OutOfScreenSystem ), std::make_unique<OutOfScreenSystem>( 97 ) );

      loadAssets();
      loadSceneData();
      createWaves();
      if ( _scene_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         factory.createEntities( getGameContext(), _scene_data.at( "Entities" ) );
      }

      _waves[ 0 ]->start( getGameContext() );
   }
   TestScene::~TestScene() {}
   void TestScene::proc()
   {
      auto delta_time = getGameContext().getGameTimer().getDeltaTime();
      _scene_elapsed_time += delta_time;

      _waves[ 0 ]->update( getGameContext() );
   }
   void TestScene::loadAssets()
   {
      auto& renderer         = getGameContext().getRenderer();
      auto& resource_manager = getGameContext().getResourceManager();
      resource_manager.loadResources( renderer, "assets/test_assets.json" );
   }
   void TestScene::loadSceneData()
   {
      // jsonからシーンデータを読み込む
      _scene_data = sdl_engine::loadJson( "game_data/scene_data/game_scene_data.json" );
   }
   void TestScene::createWaves()
   {
      // jsonからWaveを生成
      if ( _scene_data.contains( "Waves" ) )
      {
         for ( const auto& wave_data : _scene_data[ "Waves" ] )
         {
            // クラス名とデータパスを取得
            auto wave_class_name = sdl_engine::getJsonData<std::string>( wave_data, "wave_class_name" );
            auto wave_data_path  = sdl_engine::getJsonData<std::string>( wave_data, "wave_data_path" );

            auto wave = createWave( wave_class_name );

            wave->loadWaveData( wave_data_path );
            _waves.emplace_back( std::move( wave ) );
         }
      }
   }

}    // namespace myge
