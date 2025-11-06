#pragma once
#include <engine/core/forward_declarations.hpp>

namespace myge
{
   class Game
   {
   public:
      Game();
      ~Game();
      void run();

   private:
      std::unique_ptr<sdl_engine::EngineContext> _context;
   };
}    // namespace myge