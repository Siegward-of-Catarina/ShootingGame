#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
// system
#include <app/components/affiliation.hpp>
#include <app/systems/highlight_system.hpp>
#include <app/systems/menu_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <engine/core.hpp>
// event
#include <app/event/title_ui_event.hpp>

myge::TitleScene::TitleScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_, "game_data/scene_data/title_scene_data.json" }, _scene_elapsed_time { 0 }
{
}

myge::TitleScene::~TitleScene()
{
   systemManager().removeSystem<SpriteBrinkSystem>();
   systemManager().removeSystem<ScreenBoundsSystem>();
   systemManager().removeSystem<HighlightSystem>();
   systemManager().removeSystem<MenuSystem>();
   systemManager().removeSystem<OutOfScreenSystem>();
   auto& reg { registry() };
   for ( auto entt : reg.view<AffilTitleScene>() )
   {
      if ( reg.valid( entt ) ) { reg.destroy( entt ); }
   }
}

void myge::TitleScene::start() {}

void myge::TitleScene::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ ) {}

void myge::TitleScene::addSystems()
{
   systemManager().addSystem( std::make_unique<SpriteBrinkSystem>( 94, registry() ) );
   systemManager().addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
   systemManager().addSystem( std::make_unique<HighlightSystem>( 95, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<MenuSystem>( 96, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
}

void myge::TitleScene::onTitleMenuAction( const TitleMenuEvent& e )
{

   switch ( e.button_type )
   {
      case ButtonUI::Type::Start :
         sceneManager().setNextScene( std::make_unique<TestScene>( sceneDependencies() ) );
         break;
      case ButtonUI::Type::Exit : sceneManager().quitGame(); break;
   }
}

void myge::TitleScene::createEntities()
{
   auto& scene_data { sceneData() };
   if ( auto entity_data { sdl_engine::getJsonData<json>( scene_data, "Entities" ) }; entity_data )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createEntities( entity_data.value(), typeid( AffilTitleScene ) );
   }
}

void myge::TitleScene::setupEventHandlers()
{
   eventListener().connect<&TitleScene::onTitleMenuAction, TitleMenuEvent>( this );
}
