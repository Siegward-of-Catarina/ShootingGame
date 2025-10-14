#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
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
   TestScene::~TestScene() { _disp.clear(); }
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

      for ( auto entity : entities() )
      {
         if ( registry().all_of<sdl_engine::RenderUITag>( entity ) )
         {
            fade = entity;
            break;
         }
      }

      _disp.sink<ShootEvent>().connect<&TestScene::onShoot>( this );
      scene_state = SceneState::WaveStart;
   }
   void TestScene::start() {}

   void TestScene::update( f32 deita_time_ )
   {
      _scene_elapsed_time += deita_time_;
      // かり
      static int idx = 0;
      auto&      fade_comp { registry().get<sdl_engine::Fade>( fade ) };
      switch ( scene_state )
      {
         case SceneState::WaveStart :

            _waves[ idx ]->start();
            scene_state = SceneState::Wave;

            break;
         case SceneState::Wave :

            _waves[ idx ]->update( deita_time_ );
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
      system_manager.addSystem( std::make_unique<OutOfScreenSystem>( 97, registry() ) );
   }

}    // namespace myge
