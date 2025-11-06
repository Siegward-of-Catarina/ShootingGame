#include <pch.hpp>
// my header
#include <app/scene/game_scene.hpp>
// scene
#include <app/scene/game_clear_scene.hpp>
#include <app/scene/game_over_scene.hpp>
// wave
#include <app/waves/wave.hpp>
// factory
#include <app/entity_factory.hpp>
#include <app/waves/wave_factory.hpp>
// component
#include <app/components/affiliation.hpp>
#include <app/components/se_properties.hpp>
#include <app/components/shooter.hpp>
#include <app/components/status.hpp>
#include <engine/basic_component.hpp>
// system
#include <app/systems/collision_system.hpp>
#include <app/systems/enemy_ai_system.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/systems/facing_system.hpp>
#include <app/systems/hit_resolution_system.hpp>
#include <app/systems/life_ui_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/player_movement_system.hpp>
#include <app/systems/score_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/shoot_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <app/systems/transform_link_system.hpp>
// event
#include <app/events/append_charge_effect_event.hpp>
#include <app/events/append_dead_effect_event.hpp>
#include <app/events/dead_event.hpp>
#include <app/events/game_end_event.hpp>
#include <app/events/play_damage_se_event.hpp>
#include <app/events/shoot_event.hpp>
#include <engine/events/fade_events.hpp>
#include <engine/events/sound_events.hpp>
// engine
#include <engine/core.hpp>
// session
#include <app/session/score_session.hpp>

namespace
{
   // シーン内の進行状態
   enum class SceneState
   {
      WaveStart,    // Wave開始
      Wave,         // Wave更新中
      WaveEnd,      // 次のWaveへ進める
      End,          // すべてのWave終了
      GameOver,     // ゲームオーバー遷移
      GameClear     // ゲームクリア遷移
   } scene_state;
}    // namespace

namespace myge
{
   GameScene::GameScene( const sdl_engine::SceneDependencies& dependencies_ )
     : Scene { dependencies_, "game_data/scene_data/game_scene_data.json" }
     , _player_entity { entt::null }
     , _game_end_timer { 0.0f }    // ゲームエンドで値付与　カウントダウン方式で使用
     , _game_cleared { false }
     , _current_wave_index { 0 }
   {
   }

   GameScene::~GameScene()
   {
      // 追加したシステムの登録解除
      auto& system_manager { systemManager() };
      system_manager.removeSystem<PlayerMovementSystem>();
      system_manager.removeSystem<ShootSystem>();
      system_manager.removeSystem<FacingSystem>();
      system_manager.removeSystem<EnemyMovementSystem>();
      system_manager.removeSystem<EnemyBossAISystem>();
      system_manager.removeSystem<TransformLinkSystem>();
      system_manager.removeSystem<ScreenBoundsSystem>();
      system_manager.removeSystem<CollisionSystem>();
      system_manager.removeSystem<HitResolutionSystem>();
      system_manager.removeSystem<OutOfScreenSystem>();
      system_manager.removeSystem<SpriteBrinkSystem>();
      system_manager.removeSystem<LifeUISystem>();
      system_manager.removeSystem<ScoreSystem>();

      // 本シーン所属のエンティティを破棄
      auto& reg { registry() };
      for ( auto entt : reg.view<AffilGameScene>() )
      {
         if ( reg.valid( entt ) ) { reg.destroy( entt ); }
      }
   }

   void GameScene::start() {}    // 開始時処理（現在は無し）

   void GameScene::update( const sdl_engine::FrameData& frame_ )
   {

      // Wave進行とGameSceneのステートマシン
      switch ( scene_state )
      {
         case SceneState::WaveStart :
            // 現在のWaveを開始
            _waves[ _current_wave_index ]->start( _player_entity );
            scene_state = SceneState::Wave;
            break;

         case SceneState::Wave :
            // Wave更新と終了判定
            _waves[ _current_wave_index ]->update( frame_.delta_time );
            if ( _waves[ _current_wave_index ]->isWaveEnd() ) { scene_state = SceneState::WaveEnd; }
            break;

         case SceneState::WaveEnd :
            // 次のWaveへ（最後まで到達したらEndへ）
            _current_wave_index++;    // 次へ
            if ( _waves.size() == _current_wave_index )
            {
               scene_state         = SceneState::End;
               _current_wave_index = 0;
            }
            else { scene_state = SceneState::WaveStart; }
            break;

         case SceneState::End :
            if ( _game_end_timer < 0.0f )
            {
               if ( _game_cleared ) { scene_state = SceneState::GameClear; }
               else { scene_state = SceneState::GameOver; }

               // ゲームスピードを元に戻す
               sceneManager().setGameSpeed( 1.0f );
            }
            else { _game_end_timer -= frame_.delta_time; }
            break;

         case SceneState::GameOver :
            // サウンドをフェードアウトしてゲームへ
            eventListener().trigger<sdl_engine::StopAllSoundEvent>( { 100 } );
            // GameOverシーンへ遷移
            sceneManager().setNextScene( std::make_unique<GameOverScene>( sceneDependencies() ) );
            break;
         case SceneState::GameClear :
            // サウンドをフェードアウトしてゲームへ
            eventListener().trigger<sdl_engine::StopAllSoundEvent>( { 100 } );
            // GameClearシーンへ遷移 （現状GameOverシーンへ）
            sceneManager().setNextScene( std::make_unique<GameClearScene>( sceneDependencies() ) );
            break;
      }
   }

   void GameScene::createWaves()
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

   void GameScene::createEntities()
   {
      // 事前にWaveを構築
      createWaves();

      // シーンJSONのEntitiesからエンティティを生成
      auto& scene_data { sceneData() };
      if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
      {
         EntityFactory factory { registry(), resourceManager() };
         if ( auto player_data { sdl_engine::tryGetJson( data->get(), "player" ) }; player_data )
         {
            // プレイヤーを先に生成しておく ここがおかしい
            _player_entity = factory.createPlayer( player_data->get()[ 0 ], typeid( AffilGameScene ) );
         }

         factory.createEntities( *data, typeid( AffilGameScene ), entt::null, { "player" } );
      }
   }

   void GameScene::postEntityCreation()
   {
      // スコアセッションを初期化
      registry().ctx().get<ScoreSession>().value = 0u;
   }

   void GameScene::postSystemAddition()
   {
      // シーン開始時の状態設定
      scene_state = SceneState::WaveStart;
      // player life ui 初期化イベント発行
      auto status { registry().try_get<Status>( _player_entity ) };
      eventListener().trigger<PlayerLifeChangedEvent>( { status->hp } );
   }

   void GameScene::setupEventHandlers()
   {
      // 必要なイベントを購読
      eventListener().connect<&GameScene::onShoot, ShootEvent>( this );
      eventListener().connect<&GameScene::onShootLaser, LaserShootEvent>( this );
      eventListener().connect<&GameScene::onDeadEffectAppend, AppendDeadEffectEvent>( this );
      eventListener().connect<&GameScene::onGameEnd, GameEndEvent>( this );
      eventListener().connect<&GameScene::onChargeEffectAppend, AppendChargeEvent>( this );
      eventListener().connect<&GameScene::onPlayDamageSE, PlayDamageSEEvent>( this );
   }

   void GameScene::addSystems()
   {
      // 実行順（第1引数の番号が小さいほど先に実行）を指定してシステム登録
      auto& system_manager { systemManager() };
      system_manager.addSystem( std::make_unique<PlayerMovementSystem>( 1, registry() ) );
      system_manager.addSystem( std::make_unique<ShootSystem>( 2, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<FacingSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<EnemyMovementSystem>( 2, registry() ) );
      system_manager.addSystem( std::make_unique<EnemyBossAISystem>( 3, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<TransformLinkSystem>( 92, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<ScreenBoundsSystem>( 93, registry() ) );
      system_manager.addSystem( std::make_unique<CollisionSystem>( 94, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<HitResolutionSystem>( 95, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<OutOfScreenSystem>( 96, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<SpriteBrinkSystem>( 97, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<LifeUISystem>( 98, registry(), eventListener() ) );
      system_manager.addSystem( std::make_unique<ScoreSystem>( 98, registry(), eventListener() ) );
   }

   void GameScene::onShoot( ShootEvent& e )
   {
      // 弾を生成し、効果音を再生
      EntityFactory factory { registry(), resourceManager() };
      factory.createBullet( e.shooter, typeid( AffilGameScene ) );
      // シューターの種類に応じて効果音を再生
      auto         shoot_se = registry().try_get<ShootSE>( e.shooter );
      entt::entity sound { entt::null };
      // 1つのShooterの場合
      if ( shoot_se ) { sound = factory.createSoundEffect( shoot_se->sound_key, 0, shoot_se->volume, 0.0f ); }
      else    // MultipleShooterの場合
      {
         auto mul_shooter_comp = registry().try_get<MultipleShooter>( e.shooter );
         auto mul_shoot_se     = registry().try_get<MultipleShootSE>( e.shooter );
         if ( mul_shooter_comp )
         {
            // CurrentIndexに対応したShootSEを取得して再生
            auto& shoot_se_comp = mul_shoot_se->shoot_ses[ mul_shooter_comp->current_index ];
            sound               = factory.createSoundEffect( shoot_se_comp.sound_key, 0, shoot_se_comp.volume, 0.0f );
         }
      }
   }

   void GameScene::onShootLaser( LaserShootEvent& e )
   {
      // 弾を生成し、効果音を再生
      EntityFactory factory { registry(), resourceManager() };
      factory.createLaserBeam( e.shooter, typeid( AffilGameScene ) );
      auto shoot_se = registry().try_get<EXShootSE>( e.shooter );
      factory.createSoundEffect( shoot_se->sound_key, 0, shoot_se->volume, 0.0f );
   }

   void GameScene::onGameEnd( GameEndEvent& e )
   {
      // ゲームオーバー時はスローモーションへ
      sceneManager().setGameSpeed( 0.5f );
      // SEをフェードアウト
      eventListener().trigger<sdl_engine::StopSEEvent>( { 1000 } );
      _game_end_timer = 1.5f;
      scene_state     = SceneState::End;
      _game_cleared   = e.is_cleared;
   }

   void GameScene::onDeadEffectAppend( AppendDeadEffectEvent& e )
   {
      // 対象にヒットエフェクトを付与してからDeadEventを発火
      auto& reg { registry() };
      for ( auto& entity : e.dead_entities )
      {
         if ( reg.valid( entity ) && reg.all_of<sdl_engine::Transform>( entity ) )
         {
            EntityFactory factory { registry(), resourceManager() };

            factory.createDeadEffect( entity, typeid( AffilGameScene ) );

            if ( auto dead_se { reg.try_get<DeadSE>( entity ) }; dead_se )
            {
               factory.createSoundEffect( dead_se->sound_key, 0, dead_se->volume, 0.0f );
            }
         }
      }
      eventListener().trigger<DeadEvent>( { e.dead_entities, false } );
   }
   void GameScene::onChargeEffectAppend( AppendChargeEvent& e )
   {    // 弾を生成し、効果音を再生
      EntityFactory         factory { registry(), resourceManager() };
      sdl_engine::Transform trfm { e.position };
      factory.createChargeEffect( trfm, typeid( AffilGameScene ) );
   }
   void GameScene::onPlayDamageSE( PlayDamageSEEvent& e )
   {
      auto& reg { registry() };
      for ( auto entity : e.damaged_entities )
      {
         if ( auto se { reg.try_get<DamageSE>( entity ) }; se )
         {
            EntityFactory factory { registry(), resourceManager() };
            factory.createSoundEffect( se->sound_key, 0, se->volume, 0.0f );
         }
      }
   }
}    // namespace myge
