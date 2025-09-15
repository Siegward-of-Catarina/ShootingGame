#pragma once
#include "sequencer.hpp"

#include <memory>
namespace myge
{
   class Sequencer;
   class Game;
   class Scene
   {
   public:
      Scene( Game* game_ );
      virtual ~Scene() {};
      virtual void proc( float _delta ) = 0;

      Sequencer* sequencer();
      Game*      game();

   private:
      Game* _game;
   };
}    // namespace myge