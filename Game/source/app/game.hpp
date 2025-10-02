#pragma once
#include <engine/forward.hpp>

namespace myge
{
   class Game
   {
   public:
      Game();
      ~Game();
      void run();

   private:
      std::unique_ptr<sdl_engine::GameContext> _context;
   };
}    // namespace myge