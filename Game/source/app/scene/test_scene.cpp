#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/components/affiliation.hpp>
#include <app/systems/collision_system.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/systems/facing_system.hpp>
#include <app/systems/hit_resolution_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/player_movement_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/shoot_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
// event
#include <app/event/append_dead_effect_event.hpp>
#include <app/event/dead_event.hpp>
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
     : Scene { dependencies_, "game_data/scene_data/game_scene_data.json" }, _scene_elapsed_time { 0.f }
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
      system_manager.removeSystem<HitResolutionSystem>();
      system_manager.removeSystem<OutOfScreenSystem>();
      system_manager.removeSystem<SpriteBrinkSystem>();
      auto& reg { registry() };
      for ( auto entt : reg.view<AffilGameScene>() )
      {
         if ( reg.valid( entt ) ) { reg.destroy( entt ); }
      }
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
      if ( auto data { sdl_engine::tryGetJson( scene_data, "Waves" ) }; data )
      {
         for ( const auto& wave_data : data->get() )
         {
            // クラス名とデータパスを取得
            auto wave_class_name = sdl_engine::getRequireData<std::string>( wave_data, "wave_class_name" );
            auto wave_data_path  = sdl_engine::getRequireData<std::string>( wave_data, "wave_data_path" );

            auto wave = createWave( wave_class_name, depend );

            wave->loadWaveData( wave_data_path );
            _waves.emplace_back( std::move( wave ) );
         }
      }
   }

   void TestScene::createEntities()
   {
      createWaves();
      auto& scene_data { sceneData() };
      if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
      {
         EntityFactory factory { registry(), resourceManager() };
         factory.createEntities( *data, typeid( AffilGameScene ) );
      }
   }

   void TestScene::postSystemAddition() { scene_state = SceneState::WaveStart; }

   void TestScene::setupEventHandlers()
   {
      eventListener().connect<&TestScene::onShoot, ShootEvent>( this );
      eventListener().connect<&TestScene::onDeadEffectAppend, AppedDeadEffectEvent>( this );
   }

   void TestScene::addSystems()
   {
      auto& system_manager { systemManager() };
      system_manager.addSystem( std::make_unique<PlayerMovementSystem>( 1, registry() ) );
      system_manager.addSystem( std::make_unique<ShootSystem>( 2, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<FacingSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<EnemyMovementSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
      system_manager.addSystem( std::make_unique<CollisionSystem>( 95, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<HitResolutionSystem>( 96, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<SpriteBrinkSystem>( 98, registry() ) );
   }

   void TestScene::onShoot( ShootEvent& e )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createBullet( e.shooter, typeid( AffilGameScene ) );
   }

   void TestScene::onDeadEffectAppend( AppedDeadEffectEvent& e )
   {
      auto& reg { registry() };
      for ( auto& entt : e.dead_entities )
      {
         if ( reg.valid( entt ) && reg.all_of<sdl_engine::Transform>( entt ) )
         {
            EntityFactory factory { registry(), resourceManager() };
            factory.createHitEffect( entt, typeid( AffilGameScene ) );
         }
      }
      eventListener().trigger<DeadEvent>( { e.dead_entities } );
   }
}    // namespace myge
