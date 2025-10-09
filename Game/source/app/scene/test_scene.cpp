#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
// engine
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace
{
   enum class SceneState
   {
      WaveStart,
      Wave,
      WaveEnd,
      End
   } scene_state;
}

namespace myge
{
   TestScene::TestScene( sdl_engine::GameContext& ctx_ ) : Scene { ctx_ }, _scene_elapsed_time { 0.f } {}
   TestScene::~TestScene() {}
   void TestScene::initialize()
   {
      loadSceneData( "game_data/scene_data/game_scene_data.json" );
      addSystems();
      createWaves();

      auto& scene_data { getSceneData() };
      if ( scene_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         setEntities( factory.createEntities( getGameContext(), scene_data.at( "Entities" ) ) );
      }

      scene_state = SceneState::WaveStart;
   }
   void TestScene::start() {}

   void TestScene::update()
   {
      auto delta_time = getGameContext().getGameTimer().getDeltaTime();
      _scene_elapsed_time += delta_time;
      // かり
      static int idx = 0;
      switch ( scene_state )
      {
         case SceneState::WaveStart :

            _waves[ 0 ]->start( getGameContext() );
            scene_state = SceneState::Wave;

            break;
         case SceneState::Wave :

            _waves[ 0 ]->update( getGameContext() );
            if ( _waves[ 0 ]->isWaveEnd() ) { scene_state = SceneState::WaveEnd; }

            break;

         case SceneState::WaveEnd :

            idx++;    // 次へ
            if ( _waves.size() == idx ) { scene_state = SceneState::End; }
            break;

         case SceneState::End :

            getGameContext().getSceneManager().setNextScene( std::make_unique<TestScene>( getGameContext() ) );
            break;
      }
   }

   void TestScene::createWaves()
   {
      // jsonからWaveを生成
      auto& scene_data { getSceneData() };
      if ( scene_data.contains( "Waves" ) )
      {
         for ( const auto& wave_data : scene_data[ "Waves" ] )
         {
            // クラス名とデータパスを取得
            auto wave_class_name = sdl_engine::getJsonData<std::string>( wave_data, "wave_class_name" ).value();
            auto wave_data_path  = sdl_engine::getJsonData<std::string>( wave_data, "wave_data_path" ).value();

            auto wave = createWave( wave_class_name );

            wave->loadWaveData( wave_data_path );
            _waves.emplace_back( std::move( wave ) );
         }
      }
   }

   void TestScene::addSystems()
   {
      auto& system_manager { getGameContext().getSystemManager() };
      system_manager.addSystem( typeid( InputSystem ), std::make_unique<InputSystem>( 0 ) );
      system_manager.addSystem( typeid( ScreenBoundsSystem ), std::make_unique<ScreenBoundsSystem>( 95 ) );
      system_manager.addSystem( typeid( OutOfScreenSystem ), std::make_unique<OutOfScreenSystem>( 97 ) );
   }

}    // namespace myge
