#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class SceneManager
   {
   public:
      SceneManager() : _current_scene { nullptr } {};
      ~SceneManager() {};
      void   deleteCurrentScene() { _current_scene.reset(); }
      void   setCurrentScene( Scene* const current_ ) { _current_scene.reset( current_ ); };
      void   setNextScene( std::unique_ptr<Scene> next_ ) { _current_scene = std::move( next_ ); };
      Scene& currentScene() { return *_current_scene; };

   private:
      std::unique_ptr<Scene> _current_scene;
   };
}    // namespace sdl_engine