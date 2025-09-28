#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/core/game_context.hpp>

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
      u64                                      _last_time;
   };
}    // namespace myge