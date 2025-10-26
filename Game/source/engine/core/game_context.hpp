#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math/vector2.hpp>
#include <entt/entt.hpp>

namespace sdl_engine
{
   using SDL_WindowPtr = std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )>;

   struct QuitEvent;

   // フレーム共通データ
   struct FrameData
   {
      f32 delta_time;
      f32 window_width;
      f32 window_height;
   };

   // エンジン初期化パラメータ
   struct EngineInitInfo
   {
      std::string_view window_name;
      i32              window_width;
      i32              window_height;
      u8               se_track_size;
   };

   // エンジン中核のコンテキスト
   // - SDL 初期化/ウィンドウ生成/レンダラ/ミキサ等の管理
   // - Update ループで Scene/System/Renderer/Input を順に更新
   class EngineContext
   {
   public:
      EngineContext( EngineInitInfo& info_ );
      ~EngineContext();

      void update();    // 1フレームの更新
      void loadAssets( std::string_view assets_path_ );
      bool isQuit() { return is_quit; };

      // 各マネージャ/状態へのアクセサ
      Vector2_i32&      getWindowSize() { return _window_size; }
      SceneManager&     getSceneManager() { return *_scene_manager; }
      ResourceManager&  getResourceManager() { return *_resource_manager; }
      InputManager&     getInputManager() { return *_input_manager; }
      SystemManager&    getSystemManager() { return *_system_manager; }
      EventListener&    getEventListener() { return *_event_listener; }
      GameTimer&        getGameTimer() { return *_game_timer; }
      entt::registry&   getRegistry() { return _registry; }
      entt::dispatcher& getDispatcher() { return _dispatcher; }

      // 現在の依存を束ねて Scene 向けに渡す
      SceneDependencies getSceneDependencies();

      // 現在フレーム情報
      FrameData framgeData();

   private:
      // Quit イベントで終了フラグを立てる
      void onQuitEvent( QuitEvent& e );

   private:
      // 依存関係的にこの順で宣言する
      entt::dispatcher                 _dispatcher;
      entt::registry                   _registry;
      SDL_WindowPtr                    _window;
      std::unique_ptr<Renderer>        _renderer;
      std::unique_ptr<SoundMixer>      _soundmixer;
      std::unique_ptr<ResourceManager> _resource_manager;
      std::unique_ptr<SystemManager>   _system_manager;
      std::unique_ptr<SceneManager>    _scene_manager;
      std::unique_ptr<EventListener>   _event_listener;
      std::unique_ptr<InputManager>    _input_manager;

      std::unique_ptr<GameTimer> _game_timer;
      Vector2_i32                _window_size;
      bool                       is_quit;
   };
}    // namespace sdl_engine