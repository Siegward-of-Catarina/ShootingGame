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
#include <engine/rendering/loader/sprite_resource.hpp>

namespace myge
{
   TestScene::TestScene( sdl_engine::GameContext& ctx_ ) : Scene { ctx_ }
   {
      loadAssets();
      loadSceneData();
      createWaves();
      createBackGrounds();

      _waves[ 0 ]->start( _registry, getGameContext() );
   }
   TestScene::~TestScene() {}
   void TestScene::proc( f32 delta_time )
   {
      auto& renderer = getGameContext().getRenderer();

      _waves[ 0 ]->update( _registry, getGameContext(), delta_time );

      renderer.setRenderClearColor( .3f, .3f, .3f, 1.f );
      renderer.RenderClear();

      for ( auto [ entity, sprt, trfm ] : _registry.view<sdl_engine::Sprite, sdl_engine::Transform>().each() )
      {

         renderer.RenderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      }

      if ( getGameContext().getInputManager().isKeyDown( SDL_SCANCODE_Z ) ) { SDL_Log( " Z key down!" ); }
      if ( getGameContext().getInputManager().isKeyDown( SDL_SCANCODE_X ) ) { SDL_Log( " X key down!" ); }
      if ( getGameContext().getInputManager().isKeyDown( SDL_SCANCODE_UP ) ) { SDL_Log( " UP key down!" ); }
      if ( getGameContext().getInputManager().isKeyPress( SDL_SCANCODE_DOWN ) ) { SDL_Log( " DOWN key press!" ); }
      if ( getGameContext().getInputManager().isKeyUp( SDL_SCANCODE_Z ) ) { SDL_Log( " Z key up!" ); }

      renderer.RenderPresent();
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
   void TestScene::createBackGrounds()
   {
      // jsonからBackGroundを生成
      EntityFactory factory;
      if ( _scene_data.contains( "BackGrounds" ) )
      {
         for ( const auto bd_data : _scene_data[ "BackGrounds" ] )
         {
            auto back = factory.createBackGround( _registry, getGameContext().getResourceManager(), bd_data );
         }
      }
   }
}    // namespace myge
