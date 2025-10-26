#pragma once

#include <concepts>
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>

namespace sdl_engine
{
   // シーン生成に必要な依存（DI）
   struct SceneDependencies
   {
      entt::registry&   registry;
      entt::dispatcher& dispatcher;
      ResourceManager&  resource_manager;
      InputManager&     input_manager;
      SceneManager&     scene_manager;
      SystemManager&    system_manager;
   };

   // シーンの基底クラス
   // - initialize() 内でロード→エンティティ作成→システム追加→イベント購読の順を固定化
   class Scene
   {
   public:
      Scene( const SceneDependencies& dependencies_, const std::string_view _data_path );
      virtual ~Scene();

      // 初期化順を固定（final）
      virtual void initialize() final;

      // [任意] Entity 作成後の追加処理
      virtual void postEntityCreation() {};

      // [任意] System 追加後の追加処理
      virtual void postSystemAddition() {};

      // [任意] イベントハンドラの設定
      virtual void setupEventHandlers() {};

   public:
      // 開始/更新
      virtual void start()                           = 0;
      virtual void update( const FrameData& frame_ ) = 0;

   protected:
      // 派生で実装：エンティティ生成とシステム追加
      virtual void createEntities() = 0;
      virtual void addSystems()     = 0;

   protected:
      // 依存オブジェクト/データへのアクセサ
      json&             sceneData() { return _scene_data; }
      entt::registry&   registry() { return _registry; };
      EventListener&    eventListener() { return *_event_listener; };
      ResourceManager&  resourceManager() { return _resource_manager; };
      InputManager&     inputManager() { return _input_manager; }
      SceneManager&     sceneManager() { return _scene_manager; };
      SystemManager&    systemManager() { return _system_manager; };
      SceneDependencies sceneDependencies();

   private:
      // JSON シーンデータの読み込み
      void loadSceneData();

   private:
      // 依存の参照
      entt::registry&  _registry;
      ResourceManager& _resource_manager;
      InputManager&    _input_manager;
      SceneManager&    _scene_manager;
      SystemManager&   _system_manager;

      // 内部所有
      std::unique_ptr<EventListener> _event_listener;
      json                           _scene_data;
      std::string                    _data_path;
   };
}    // namespace sdl_engine