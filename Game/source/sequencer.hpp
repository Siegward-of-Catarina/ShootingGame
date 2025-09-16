#pragma once
#include <core/fwd.hpp>
namespace myge
{
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