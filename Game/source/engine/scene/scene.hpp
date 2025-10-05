#pragma once

#include <engine/core/forward_declarations.hpp>

namespace sdl_engine
{
   class Scene
   {
   public:
      Scene( GameContext& ctx_ );
      virtual ~Scene() {};
      virtual void initialize() = 0;
      virtual void start()      = 0;
      virtual void update()     = 0;

      GameContext& getGameContext() { return _context; }

   private:
      GameContext& _context;
   };
}    // namespace sdl_engine