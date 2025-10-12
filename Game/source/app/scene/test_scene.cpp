#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/systems/enemy_movement_system.hpp>
#include <app/systems/input_system.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/player_movement_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/shoot_system.hpp>
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
      for ( auto entity : getEntities() )
      {
         if ( registry.all_of<sdl_engine::RenderUITag>( entity ) )
         {
            fade = entity;
            break;
         }
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
      auto&      registry { getGameContext().getRegistry() };
      auto&      fade_comp { registry.get<sdl_engine::Fade>( fade ) };
      switch ( scene_state )
      {
         case SceneState::WaveStart :

            _waves[ idx ]->start( getGameContext() );
            scene_state = SceneState::Wave;

            break;
         case SceneState::Wave :

            _waves[ idx ]->update( getGameContext() );
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

            if ( fade_comp.state == sdl_engine::Fade::State::Idel )
            {
               fade_comp.state = sdl_engine::Fade::State::FadeOut;
            }
            if ( fade_comp.state == sdl_engine::Fade::State::FadeOutEnd )
            {
               if ( getGameContext().getInputManager().isAnyKeydown() ) { scene_state = SceneState::Destroy; }
            }
            break;
         case SceneState::Destroy :
            getGameContext().getSceneManager().setNextScene( std::make_unique<TitleScene>( getGameContext() ) );
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
      system_manager.addSystem( typeid( PlayerMovementSystem ), std::make_unique<PlayerMovementSystem>( 1 ) );
      system_manager.addSystem( typeid( ShootSystem ), std::make_unique<ShootSystem>( 2 ) );
      system_manager.addSystem( typeid( EnemyMovementSystem ), std::make_unique<EnemyMovementSystem>( 2 ) );
      system_manager.addSystem( typeid( ScreenBoundsSystem ), std::make_unique<ScreenBoundsSystem>( 95 ) );
      system_manager.addSystem( typeid( OutOfScreenSystem ), std::make_unique<OutOfScreenSystem>( 97 ) );
   }

}    // namespace myge
