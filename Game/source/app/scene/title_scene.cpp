#include <app/components/title_input.hpp>
#include <app/entity_factory.hpp>
#include <app/scene/test_scene.hpp>
#include <app/scene/title_scene.hpp>
// system
#include <app/systems/input_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace
{
   entt::entity title_input {};
}
myge::TitleScene::TitleScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_ }, _scene_elapsed_time { 0 }
{
}

myge::TitleScene::~TitleScene() {}

void myge::TitleScene::initialize()
{
   loadSceneData( "game_data/scene_data/title_scene_data.json" );
   auto& scene_data { sceneData() };
   if ( scene_data.contains( "Entities" ) )
   {
       EntityFactory factory{ registry(), resourceManager()};
      setEntities( factory.createEntities( scene_data.at( "Entities" ) ) );
   }
   for ( auto& entity : entities() )
   {
      if ( registry().all_of<TitleInput>( entity ) ) { title_input = entity; }
   }

   addSystems();
}

void myge::TitleScene::start() {}

void myge::TitleScene::update( f32 deita_time_ )
{

   if ( registry().get<TitleInput>( title_input ).any_key )
   {
      sceneManager().setNextScene( std::make_unique<TestScene>( sceneDependencies() ) );
   }
}

void myge::TitleScene::addSystems()
{
   systemManager().addSystem( std::make_unique<SpriteBrinkSystem>( 94, registry() ) );
   systemManager().addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry() ) );
}
