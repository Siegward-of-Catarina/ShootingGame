// ゲーム本編のテストシーン。
// - Waveの開始/更新/終了を状態機械で管理
// - 各種システムの登録とイベントハンドラの受け口を提供
#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/game_over_scene.hpp>
#include <app/scene/test_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
// system
#include <app/components/affiliation.hpp>
#include <app/systems/collision_system.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/systems/facing_system.hpp>
#include <app/systems/game_over_system.hpp>
#include <app/systems/hit_resolution_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/player_movement_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/shoot_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <app/systems/transform_link_system.hpp>
// event
#include <app/event/append_dead_effect_event.hpp>
#include <app/event/append_overray_fade_event.hpp>
#include <app/event/dead_event.hpp>
#include <app/event/game_over_event.hpp>
#include <app/event/shoot_event.hpp>
#include <engine/events/sound_events.hpp>
// engine
#include <app/event/game_over_event.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/events/fade_events.hpp>

//
#include <SDL3_mixer/SDL_mixer.h>

namespace
{
   // シーン内の進行状態
   enum class SceneState
   {
      WaveStart,    // Wave開始
      Wave,         // Wave更新中
      WaveEnd,      // 次のWaveへ進める
      GameOver,     // 未使用（将来拡張）
      End,          // すべてのWave終了
      Destroy       // シーン遷移要求
   } scene_state;
   entt::entity fade;    // フェード制御用（将来拡張）
}    // namespace

namespace myge
{
   TestScene::TestScene( const sdl_engine::SceneDependencies& dependencies_ )
     : Scene { dependencies_, "game_data/scene_data/game_scene_data.json" }, _scene_elapsed_time { 0.f }
   {
   }

   TestScene::~TestScene()
   {
      // 追加したシステムの登録解除
      auto& system_manager { systemManager() };
      system_manager.removeSystem<PlayerMovementSystem>();
      system_manager.removeSystem<ShootSystem>();
      system_manager.removeSystem<FacingSystem>();
      system_manager.removeSystem<EnemyMovementSystem>();
      system_manager.removeSystem<TransformLinkSystem>();
      system_manager.removeSystem<ScreenBoundsSystem>();
      system_manager.removeSystem<CollisionSystem>();
      system_manager.removeSystem<HitResolutionSystem>();
      system_manager.removeSystem<OutOfScreenSystem>();
      system_manager.removeSystem<SpriteBrinkSystem>();
      system_manager.removeSystem<GameOverSystem>();

      // 本シーン所属のエンティティを破棄
      auto& reg { registry() };
      for ( auto entt : reg.view<AffilGameScene>() )
      {
         if ( reg.valid( entt ) ) { reg.destroy( entt ); }
      }
   }

   void TestScene::start() {}    // 開始時処理（現在は無し）

   void TestScene::update( const sdl_engine::FrameData& frame_ )
   {
      _scene_elapsed_time += frame_.delta_time;

      // Wave進行用のステートマシン
      static int idx = 0;    // 現在のWaveインデックス
      switch ( scene_state )
      {
         case SceneState::WaveStart :
            // 現在のWaveを開始
            _waves[ idx ]->start();
            scene_state = SceneState::Wave;
            break;

         case SceneState::Wave :
            // Wave更新と終了判定
            _waves[ idx ]->update( frame_.delta_time );
            if ( _waves[ idx ]->isWaveEnd() ) { scene_state = SceneState::WaveEnd; }
            break;

         case SceneState::WaveEnd :
            // 次のWaveへ（最後まで到達したらEndへ）
            idx++;    // 次へ
            if ( _waves.size() == idx )
            {
               scene_state = SceneState::End;
               idx         = 0;
            }
            else { scene_state = SceneState::WaveStart; }
            break;

         case SceneState::End :
            // 任意キーでシーン終了フローへ
            if ( inputManager().isAnyKeydown() ) { scene_state = SceneState::Destroy; }
            break;

         case SceneState::Destroy :
            // GameOverシーンへ遷移
            sceneManager().setNextScene( std::make_unique<GameOverScene>( sceneDependencies() ) );
            break;

         case SceneState::GameOver :
            // 未使用
            break;
      }
   }

   void TestScene::createWaves()
   {
      // シーンJSONからWaveを生成
      auto&            scene_data { sceneData() };
      WaveDependencies depend { registry(), resourceManager() };
      if ( auto data { sdl_engine::tryGetJson( scene_data, "Waves" ) }; data )
      {
         for ( const auto& wave_data : data->get() )
         {
            // Waveのクラス名とデータパスを取得して生成
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
      // 事前にWaveを構築
      createWaves();

      // シーンJSONのEntitiesからエンティティを生成
      auto& scene_data { sceneData() };
      if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
      {
         EntityFactory factory { registry(), resourceManager() };
         factory.createEntities( *data, typeid( AffilGameScene ) );
      }
   }

   void TestScene::postSystemAddition()
   {
      // シーン開始時の状態設定
      scene_state = SceneState::WaveStart;
   }

   void TestScene::setupEventHandlers()
   {
      // 必要なイベントを購読
      eventListener().connect<&TestScene::onShoot, ShootEvent>( this );
      eventListener().connect<&TestScene::onDeadEffectAppend, AppendDeadEffectEvent>( this );
      eventListener().connect<&TestScene::onOverrayFadeAppend, AppendOverrayFadeEvent>( this );
      eventListener().connect<&TestScene::onGameOver, GameOverEvent>( this );
   }

   void TestScene::addSystems()
   {
      // 実行順（第1引数の番号が小さいほど先に実行）を指定してシステム登録
      auto& system_manager { systemManager() };
      system_manager.addSystem( std::make_unique<PlayerMovementSystem>( 1, registry() ) );
      system_manager.addSystem( std::make_unique<ShootSystem>( 2, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<FacingSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<EnemyMovementSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<TransformLinkSystem>( 93, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<ScreenBoundsSystem>( 94, registry() ) );
      system_manager.addSystem( std::make_unique<CollisionSystem>( 95, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<HitResolutionSystem>( 96, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<SpriteBrinkSystem>( 98, registry() ) );
      system_manager.addSystem( std::make_unique<GameOverSystem>( 99, registry(), eventListener() ) );
   }

   void TestScene::onShoot( ShootEvent& e )
   {
      // 弾を生成し、効果音を再生
      EntityFactory factory { registry(), resourceManager() };
      factory.createBullet( e.shooter, typeid( AffilGameScene ) );
      auto sound = factory.createSoundEffect( "se_chun1", 0, 0.5f );
      eventListener().trigger<sdl_engine::PlaySEEvent>( { sound } );
   }

   void TestScene::onGameOver( GameOverEvent& e )
   {
      // ゲームオーバー時はスローモーションへ
      sceneManager().setGameSpeed( 0.5f );
   }

   void TestScene::onDeadEffectAppend( AppendDeadEffectEvent& e )
   {
      // 対象にヒットエフェクトを付与してからDeadEventを発火
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

   void TestScene::onOverrayFadeAppend( AppendOverrayFadeEvent& e )
   {
      // フェード要求：遷移フローへ移行し、ゲーム速度を戻す
      scene_state = SceneState::Destroy;
      sceneManager().setGameSpeed( 1.0f );
   }
}    // namespace myge
