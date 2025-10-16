#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/systems/collision_system.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/systems/facing_system.hpp>
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/player_movement_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/shoot_system.hpp>
// event
#include <app/event/shoot_event.hpp>
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
      GameOver,
      End,
      Destroy
   } scene_state;
   entt::entity fade;
}    // namespace

namespace myge
{
   TestScene::TestScene( const sdl_engine::SceneDependencies& dependencies_ )
     : Scene { dependencies_ }, _scene_elapsed_time { 0.f }
   {
   }
   TestScene::~TestScene()
   {

      auto& system_manager { systemManager() };
      system_manager.removeSystem<PlayerMovementSystem>();
      system_manager.removeSystem<ShootSystem>();
      system_manager.removeSystem<FacingSystem>();
      system_manager.removeSystem<EnemyMovementSystem>();
      system_manager.removeSystem<ScreenBoundsSystem>();
      system_manager.removeSystem<CollisionSystem>();
      system_manager.removeSystem<OutOfScreenSystem>();
   }
   void TestScene::initialize()
   {
      loadSceneData( "game_data/scene_data/game_scene_data.json" );
      addSystems();
      createWaves();
      auto& scene_data { sceneData() };
      if ( scene_data.contains( "Entities" ) )
      {
         EntityFactory factory { registry(), resourceManager() };
         setEntities( factory.createEntities( scene_data.at( "Entities" ) ) );
      }
      _disp.sink<ShootEvent>().connect<&TestScene::onShoot>( this );
      scene_state = SceneState::WaveStart;
   }
   void TestScene::start() {}

   void TestScene::update( const sdl_engine::FrameData& frame_ )
   {
      _scene_elapsed_time += frame_.delta_time;
      // かり
      static int idx = 0;
      switch ( scene_state )
      {
         case SceneState::WaveStart :

            _waves[ idx ]->start();
            scene_state = SceneState::Wave;

            break;
         case SceneState::Wave :

            _waves[ idx ]->update( frame_.delta_time );
            if ( _waves[ idx ]->isWaveEnd() ) { scene_state = SceneState::WaveEnd; }

            break;

         case SceneState::WaveEnd :

            idx++;    // 次へ
            if ( _waves.size() == idx )
            {
               scene_state = SceneState::End;
               idx         = 0;
            }
            else { scene_state = SceneState::WaveStart; }
            break;
         case SceneState::End :
            if ( inputManager().isAnyKeydown() ) { scene_state = SceneState::Destroy; }
            break;
         case SceneState::Destroy :
            sceneManager().setNextScene( std::make_unique<TitleScene>( sceneDependencies() ) );
            break;
      }
   }

   void TestScene::createWaves()
   {
      // jsonからWaveを生成
      auto&            scene_data { sceneData() };
      WaveDependencies depend { registry(), resourceManager() };
      if ( scene_data.contains( "Waves" ) )
      {
         for ( const auto& wave_data : scene_data[ "Waves" ] )
         {
            // クラス名とデータパスを取得
            auto wave_class_name = sdl_engine::getJsonData<std::string>( wave_data, "wave_class_name" ).value();
            auto wave_data_path  = sdl_engine::getJsonData<std::string>( wave_data, "wave_data_path" ).value();

            auto wave = createWave( wave_class_name, depend );

            wave->loadWaveData( wave_data_path );
            _waves.emplace_back( std::move( wave ) );
         }
      }
   }

   void TestScene::onShoot( ShootEvent& e )
   {
      EntityFactory factory { registry(), resourceManager() };
      entities().emplace_back( factory.createBullet( e.shooter ) );
   }

   void TestScene::addSystems()
   {
      auto& system_manager { systemManager() };
      system_manager.addSystem( std::make_unique<PlayerMovementSystem>( 1, registry() ) );
      system_manager.addSystem( std::make_unique<ShootSystem>( 2, registry(), _disp ) );
      system_manager.addSystem( std::make_unique<FacingSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<EnemyMovementSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
      system_manager.addSystem( std::make_unique<CollisionSystem>( 95, registry(), dispatcher() ) );
      system_manager.addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), dispatcher() ) );
   }

}    // namespace myge
