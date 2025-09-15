#pragma once
#include "scene.hpp"
namespace myge
{
   class Scene;
   class Sequencer
   {
   public:
      Sequencer();
      Sequencer( Scene* current_ );
      ~Sequencer();
      void   deleteCurrentScene();
      void   setNextScene( Scene* current_ );
      Scene* currentScene();

   private:
      Scene* _current_scene;
   };
}    // namespace myge