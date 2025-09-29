#include <SDL3/SDL.h>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/waves/wave.hpp>
#include <app/waves/wave_factory.hpp>
#include <engine/components/sprite.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/utils/json_utilities.hpp>
//
#include <app/systems/out_of_screen_system.hpp>
#include <engine/rendering/loader/sprite_resource.hpp>
#include <engine/systems/sprite_animation_system.hpp>
#include <engine/systems/sprite_render_system.hpp>
#include <engine/systems/transfrom_update_system.hpp>
namespace myge
{
   TestScene::TestScene( sdl_engine::GameContext& ctx_ ) : Scene { ctx_ }, _systems {}, _scene_elapsed_time { 0.f }
   {
      _player_movement_system = std::make_unique<PlayerMovementSystem>();
      _systems.emplace_back( std::make_unique<sdl_engine::TransformUpdateSystem>() );
      _systems.emplace_back( std::make_unique<sdl_engine::SpriteRenderSystem>() );
      _systems.emplace_back( std::make_unique<sdl_engine::SpriteAnimationSystem>() );
      _systems.emplace_back( std::make_unique<OutOfScreenSystem>() );
      // 優先度でソート
      std::sort( _systems.begin(),
                 _systems.end(),
                 []( const std::unique_ptr<sdl_engine::SystemInterface>& rref,
                     const std::unique_ptr<sdl_engine::SystemInterface>& lref )
                 { return rref->priority() < lref->priority(); } );

      loadAssets();
      loadSceneData();
      createWaves();
      if ( _scene_data.contains( "Entities" ) )
      {
         EntityFactory factory;
         factory.createEntities( _registry, getGameContext().getResourceManager(), _scene_data.at( "Entities" ) );
      }

      _waves[ 0 ]->start( _registry, getGameContext() );
   }
   TestScene::~TestScene() {}
   void TestScene::proc( f32 delta_time_ )
   {
      _scene_elapsed_time += static_cast<f64>( delta_time_ );
      auto& renderer = getGameContext().getRenderer();

      _waves[ 0 ]->update( _registry, getGameContext(), delta_time_ );

      _player_movement_system->update( _registry, getGameContext(), delta_time_ );

      for ( auto& system : _systems ) { system->update( _registry, getGameContext(), delta_time_ ); }
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
         for ( const auto wave_data : _scene_data[ "Waves" ] )
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
