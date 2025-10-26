#include <engine/core.hpp>
#include <engine/events/event_listener.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/scene/scene.hpp>

namespace sdl_engine
{
   Scene::Scene( const SceneDependencies& dependencies_, const std::string_view _data_path )
     : _registry { dependencies_.registry }
     , _event_listener { nullptr }
     , _resource_manager { dependencies_.resource_manager }
     , _input_manager { dependencies_.input_manager }
     , _scene_manager { dependencies_.scene_manager }
     , _system_manager { dependencies_.system_manager }
     , _data_path { _data_path }
   {
      // シーンごとに EventListener を所有（dispatcher は共有）
      _event_listener = std::make_unique<EventListener>( dependencies_.dispatcher );
   }

   Scene::~Scene() {}

   void Scene::initialize()
   {
      // 初期化順を統一
      loadSceneData();
      createEntities();
      postEntityCreation();
      addSystems();
      postSystemAddition();
      setupEventHandlers();
   }

   SceneDependencies Scene::sceneDependencies()
   {
      // 現在のシーンが保持する依存を再度束ねて返す
      return SceneDependencies { _registry,         _event_listener->dispatcher(),
                                 _resource_manager, _input_manager,
                                 _scene_manager,    _system_manager };
   }

   void Scene::loadSceneData()
   {
      // json からシーンデータを読み込む
      _scene_data = sdl_engine::loadJson( _data_path.data() );
      std::string msg { _data_path.data() };
      msg += "をロードしました";
      SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, msg.c_str() );
   }
}    // namespace sdl_engine