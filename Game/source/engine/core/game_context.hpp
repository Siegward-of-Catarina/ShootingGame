#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math/vector2.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   struct QuitEvent;

   struct FrameData
   {
      f32 delta_time;
      f32 window_width;
      f32 window_height;
   };
   class EngineContext
   {
   public:
      EngineContext( std::string_view window_name_, i32 window_width_, i32 window_height_ );
      ~EngineContext();
      void              update();
      void              loadAssets( std::string_view assets_path_ );
      bool              isQuit() { return is_quit; };
      Vector2_i32&      getWindowSize() { return _window_size; }
      Renderer&         getRenderer() { return *_renderer; }
      SceneManager&     getSceneManager() { return *_scene_manager; }
      ResourceManager&  getResourceManager() { return *_resource_manager; }
      InputManager&     getInputManager() { return *_input_manager; }
      SystemManager&    getSystemManager() { return *_system_manager; }
      GameTimer&        getGameTimer() { return *_game_timer; }
      entt::registry&   getRegistry() { return _registry; }
      entt::dispatcher& getDispatcher() { return _dispatcher; }

      FrameData framgeData();

   private:
      void onQuitEvent( QuitEvent& e );

   private:
      entt::dispatcher                 _dispatcher;
      entt::registry                   _registry;
      SDL_WindowPtr                    _window;
      std::unique_ptr<Renderer>        _renderer;
      std::unique_ptr<ResourceManager> _resource_manager;
      std::unique_ptr<SystemManager>   _system_manager;
      std::unique_ptr<SceneManager>    _scene_manager;
      std::unique_ptr<InputManager>    _input_manager;

      std::unique_ptr<GameTimer> _game_timer;
      Vector2_i32                _window_size;
      bool                       is_quit;
   };
}    // namespace sdl_engine
