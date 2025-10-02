#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math/vector2.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{

   class GameContext
   {
   public:
      GameContext( std::string_view window_name_, i32 window_width_, i32 window_height_ );
      ~GameContext();

      const Vector2_i32& getWindowSize() const { return _window_size; }
      Renderer&          getRenderer() { return *_renderer; }
      SceneManager&      getSequencer() { return *_sequencer; }
      ResourceManager&   getResourceManager() { return *_resource_manager; }
      InputManager&      getInputManager() { return *_input_manager; }
      SystemManager&     getSystemManager() { return *_system_manager; }
      GameTimer&         getGameTimer() { return *_game_timer; }
      entt::registry&    getRegistry() { return _registry; }

   private:
      SDL_WindowPtr                    _window;
      std::unique_ptr<Renderer>        _renderer;
      std::unique_ptr<SceneManager>    _sequencer;
      std::unique_ptr<ResourceManager> _resource_manager;
      std::unique_ptr<InputManager>    _input_manager;
      std::unique_ptr<SystemManager>   _system_manager;

      std::unique_ptr<GameTimer> _game_timer;
      entt::registry             _registry;

      Vector2_i32 _window_size;
   };
}    // namespace sdl_engine
