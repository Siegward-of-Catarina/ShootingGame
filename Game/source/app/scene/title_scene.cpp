#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
// system
#include <app/systems/input_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace
{
   entt::entity title_input {};
}
myge::TitleScene::TitleScene( sdl_engine::GameContext& ctx_ ) : Scene { ctx_ }, _scene_elapsed_time { 0 } {}

myge::TitleScene::~TitleScene() {}

void myge::TitleScene::initialize()
{
   loadSceneData( "game_data/scene_data/title_scene_data.json" );
   auto& scene_data { getSceneData() };
   if ( scene_data.contains( "Entities" ) )
   {
      EntityFactory factory;
      setEntities( factory.createEntities( getGameContext(), scene_data.at( "Entities" ) ) );
   }
   for ( auto& entity : getEntities() )
   {
      if ( getGameContext().getRegistry().all_of<TitleInput>( entity ) ) { title_input = entity; }
   }
   addSystems();
}

void myge::TitleScene::start() {}

void myge::TitleScene::update()
{
   auto& registry { getGameContext().getRegistry() };

   if ( registry.get<TitleInput>( title_input ).any_key )
   {
      getGameContext().getSceneManager().setNextScene( std::make_unique<TestScene>( getGameContext() ) );
   }
}

void myge::TitleScene::addSystems()
{
   auto& system_manager { getGameContext().getSystemManager() };
   system_manager.addSystem( typeid( InputSystem ), std::make_unique<InputSystem>( 0 ) );
   system_manager.addSystem( typeid( ScreenBoundsSystem ), std::make_unique<ScreenBoundsSystem>( 95 ) );
   system_manager.addSystem( typeid( OutOfScreenSystem ), std::make_unique<OutOfScreenSystem>( 97 ) );
}
