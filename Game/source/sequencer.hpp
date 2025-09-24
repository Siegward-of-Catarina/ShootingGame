#pragma once
#include <core/fwd.hpp>
namespace myge
{
   class Sequencer
   {
   public:
      Sequencer() : _current_scene { nullptr } {};
      ~Sequencer() {};
      void   deleteCurrentScene() { _current_scene.reset(); }
      void   setCurrentScene( std::unique_ptr<Scene> current_ ) { _current_scene = std::move( current_ ); };
      void   setNextScene( std::unique_ptr<Scene> next_ ) { _current_scene = std::move( next_ ); };
      Scene& currentScene() { return *_current_scene; };

   private:
      std::unique_ptr<Scene> _current_scene;
   };
}    // namespace myge