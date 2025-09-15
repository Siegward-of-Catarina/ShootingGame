#pragma once
#include <SDL3/SDL.h>
#include <iostream>

namespace myge
{
   class Sequencer;
   class Game
   {
   public:
      Game();
      ~Game();
      bool          init();
      void          run();
      void          quit();
      SDL_Renderer* render() { return _render; }
      Sequencer*    sequencer() { return _sequencer; }

   private:
      SDL_Window*   _window;
      SDL_Renderer* _render;
      Sequencer*    _sequencer;
      std::uint32_t _last_time;
   };
}    // namespace myge