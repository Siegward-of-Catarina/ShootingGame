#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
// system
#include <app/systems/highlight_system.hpp>
#include <app/systems/input_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
// event
#include <app/event/highlight_event.hpp>
#include <engine/events/quit_event.hpp>

namespace
{
}
myge::TitleScene::TitleScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_ }, _scene_elapsed_time { 0 }, _game_start_ui {}, _exit_game_ui {}
{
}

myge::TitleScene::~TitleScene()
{
   systemManager().removeSystem<SpriteBrinkSystem>();
   systemManager().removeSystem<ScreenBoundsSystem>();
   systemManager().removeSystem<HighlightSystem>();
   systemManager().removeSystem<OutOfScreenSystem>();
}

void myge::TitleScene::initialize()
{
   loadSceneData( "game_data/scene_data/title_scene_data.json" );
   addSystems();

   auto& scene_data { sceneData() };
   if ( auto entity_data { sdl_engine::getJsonData<json>( scene_data, "Entities" ) }; entity_data )
   {
      EntityFactory factory { registry(), resourceManager() };
      // selectaable_textを除外する
      setEntities( factory.createEntities( entity_data.value(), std::vector<std::string> { "highlightable_ui" } ) );

      // highlightable_uiを個別に生成しておく
      if ( auto highlight_ui_data { sdl_engine::getJsonData<json>( entity_data.value(), "highlightable_ui" ) };
           highlight_ui_data )
      {
         auto highlight_uis { highlight_ui_data.value() };
         if ( highlight_uis.size() >= 2 )
         {
            _game_start_ui = factory.createHighlightableUI( highlight_uis[ 0 ] );
            _exit_game_ui  = factory.createHighlightableUI( highlight_uis[ 1 ] );
            entities().emplace_back( _game_start_ui );
            entities().emplace_back( _exit_game_ui );
            // 最初はゲームスタートが選択されている状態にする
            dispatcher().trigger<HighlightEvent>( { _game_start_ui } );
         }
      }
   }
}

void myge::TitleScene::start() {}

void myge::TitleScene::update( const sdl_engine::FrameData& frame_ )
{
   static int flag = 1;
   if ( inputManager().isKeyDown( SDL_SCANCODE_SPACE ) )
   {
      if ( flag ) { sceneManager().setNextScene( std::make_unique<TestScene>( sceneDependencies() ) ); }
      else { dispatcher().trigger<sdl_engine::QuitEvent>(); }
   }
   if ( inputManager().isKeyDown( SDL_SCANCODE_UP ) || inputManager().isKeyDown( SDL_SCANCODE_W ) )
   {
      dispatcher().trigger<HighlightEvent>( { _game_start_ui } );
      flag = 1;
   }
   if ( inputManager().isKeyDown( SDL_SCANCODE_DOWN ) || inputManager().isKeyDown( SDL_SCANCODE_S ) )
   {
      dispatcher().trigger<HighlightEvent>( { _exit_game_ui } );
      flag = 0;
   }
}

void myge::TitleScene::addSystems()
{
   systemManager().addSystem( std::make_unique<SpriteBrinkSystem>( 94, registry() ) );
   systemManager().addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
   systemManager().addSystem( std::make_unique<HighlightSystem>( 99, registry(), dispatcher() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), dispatcher() ) );
}
