#pragma once

#include <concepts>
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   struct SceneDependencies
   {
      entt::registry&   registry;
      entt::dispatcher& dispatcher;
      ResourceManager&  resource_manager;
      InputManager&     input_manager;
      SceneManager&     scene_manager;
      SystemManager&    system_manager;
   };
   class Scene
   {
   public:
      Scene( const SceneDependencies& dependencies_, const std::string_view _data_path );
      virtual ~Scene();

      // 初期化順は固定化するため final 指定
      virtual void initialize() final;
      // entity 作成後の処理 [オプション]　必要なときのみ override
      virtual void postEntityCreation() {};
      // system 追加後の処理 [オプション]　必要なときのみ override
      virtual void postSystemAddition() {};
      // イベントハンドラ設定 [オプション]　必要なときのみ override
      virtual void setupEventHandlers() {};

   public:
      virtual void start()                           = 0;
      virtual void update( const FrameData& frame_ ) = 0;

   protected:
      virtual void createEntities() = 0;
      virtual void addSystems()     = 0;

   protected:
      json&             sceneData() { return _scene_data; }
      entt::registry&   registry() { return _registry; };
      EventListener&    eventListener() { return *_event_listener; };
      ResourceManager&  resourceManager() { return _resource_manager; };
      InputManager&     inputManager() { return _input_manager; }
      SceneManager&     sceneManager() { return _scene_manager; };
      SystemManager&    systemManager() { return _system_manager; };
      SceneDependencies sceneDependencies();

   private:
      void loadSceneData();

   private:
      entt::registry&  _registry;
      ResourceManager& _resource_manager;
      InputManager&    _input_manager;
      SceneManager&    _scene_manager;
      SystemManager&   _system_manager;

   private:
      std::unique_ptr<EventListener> _event_listener;
      json                           _scene_data;
      std::string                    _data_path;
   };
}    // namespace sdl_engine