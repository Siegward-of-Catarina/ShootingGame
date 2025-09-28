#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math/vector2.hpp>
namespace sdl_engine
{

   class GameContext
   {
   public:
      GameContext( std::string_view window_name_, i32 window_width_, i32 window_height_ );
      ~GameContext();
      void setNextScene( std::unique_ptr<Scene> next_ );
      void setCurrentScene( Scene* const current_ );

      const Vector2_i32 getWindowSize() const { return _window_size; }
      Renderer&         getRenderer() { return *_renderer; }
      SceneManager&     getSequencer() { return *_sequencer; }
      ResourceManager&  getResourceManager() { return *_resource_manager; }
      InputManager&     getInputManager() { return *_input_manager; }

   private:
      SDL_WindowPtr                    _window;
      std::unique_ptr<Renderer>        _renderer;
      std::unique_ptr<SceneManager>    _sequencer;
      std::unique_ptr<ResourceManager> _resource_manager;
      std::unique_ptr<InputManager>    _input_manager;
      Vector2_i32                      _window_size;
   };
}    // namespace sdl_engine
