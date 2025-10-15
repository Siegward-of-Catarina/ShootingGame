#include <engine/core.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/scene/scene.hpp>
namespace sdl_engine
{
   Scene::Scene( const SceneDependencies& dependencies_ )
     : _registry { dependencies_.registry }
     , _resource_manager { dependencies_.resource_manager }
     , _input_manager { dependencies_.input_manager }
     , _scene_manager { dependencies_.scene_manager }
     , _system_manager { dependencies_.system_manager }
   {
   }
   Scene::~Scene()
   {
      for ( auto& entity : _entities )
      {
         if ( _registry.valid( entity ) ) { _registry.destroy( entity ); }
      }
   }
   void Scene::loadSceneData( std::string_view data_path_ )
   {
      // jsonからシーンデータを読み込む
      _scene_data = sdl_engine::loadJson( data_path_.data() );
      std::string msg { data_path_.data() };
      msg += "をロードしました";
      SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, msg.c_str() );
   }

}    // namespace sdl_engine