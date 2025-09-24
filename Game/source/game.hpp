#pragma once
#include <core/fwd.hpp>
namespace myge
{
   class Game
   {
   public:
      Game();
      ~Game();
      void       run();
      Renderer&  render() { return *_renderer; }
      Sequencer& sequencer() { return *_sequencer; }

   private:
      SDL_WindowPtr                    _window;
      std::unique_ptr<Renderer>        _renderer;
      std::unique_ptr<Sequencer>       _sequencer;
      std::unique_ptr<ResourceManager> _resource_manager;
      u64                              _last_time;
   };
}    // namespace myge