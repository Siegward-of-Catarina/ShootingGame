#include <app/entity_factory.hpp>
#include <app/scene/game_over_scene.hpp>
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
#include <app/event/menu_button_event.hpp>

myge::GameOverScene::GameOverScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_, "game_data/scene_data/game_over_scene_data.json" }, _scene_elapsed_time { 0 }
{
}

myge::GameOverScene::~GameOverScene()
{
   systemManager().removeSystem<HighlightSystem>();
   systemManager().removeSystem<MenuSystem>();
   systemManager().removeSystem<OutOfScreenSystem>();
   auto& reg { registry() };
   for ( auto entt : reg.view<AffilTitleScene>() )
   {
      if ( reg.valid( entt ) ) { reg.destroy( entt ); }
   }
}

void myge::GameOverScene::start() {}

void myge::GameOverScene::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ ) {}

void myge::GameOverScene::addSystems()
{
   systemManager().addSystem( std::make_unique<HighlightSystem>( 95, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<MenuSystem>( 96, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
}

void myge::GameOverScene::createEntities()
{
   auto& scene_data { sceneData() };
   if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createEntities( *data, typeid( AffilTitleScene ) );
   }
}

void myge::GameOverScene::setupEventHandlers()
{
   eventListener().connect<&GameOverScene::onContinueMenuAction, MenuButtonEvent>( this );
}

void myge::GameOverScene::onContinueMenuAction( const MenuButtonEvent& e )
{
   switch ( e.button_type )
   {
      case ButtonUI::Type::Start :
         sceneManager().setNextScene( std::make_unique<TestScene>( sceneDependencies() ) );
         break;
      case ButtonUI::Type::Exit : sceneManager().quitGame(); break;
   }
}
