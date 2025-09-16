#pragma once

#include <core/fwd.hpp>

namespace myge
{
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