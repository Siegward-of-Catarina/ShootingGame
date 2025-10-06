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
      Wave,
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

      auto& registry { getGameContext().getRegistry() };
      scene_state = SceneState::Wave;
   }
   void TestScene::start() { _waves[ 0 ]->start( getGameContext() ); }

   void TestScene::update()
   {
      auto delta_time = getGameContext().getGameTimer().getDeltaTime();
      _scene_elapsed_time += delta_time;

      switch ( scene_state )
      {
         case SceneState::Wave :
         {
            _waves[ 0 ]->update( getGameContext() );
            if ( _scene_elapsed_time > 10 ) { scene_state = SceneState::End; }
            break;
         }
         case SceneState::End :
         {
            getGameContext().getSceneManager().setNextScene( std::make_unique<TestScene>( getGameContext() ) );
            break;
         }
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
            auto wave_class_name = sdl_engine::getJsonData<std::string>( wave_data, "wave_class_name" );
            auto wave_data_path  = sdl_engine::getJsonData<std::string>( wave_data, "wave_data_path" );

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
      system_manager.addSystem( typeid( LifeCycleSystem ), std::make_unique<LifeCycleSystem>( 96 ) );
      system_manager.addSystem( typeid( OutOfScreenSystem ), std::make_unique<OutOfScreenSystem>( 97 ) );
   }

}    // namespace myge
