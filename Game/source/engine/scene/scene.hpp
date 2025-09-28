#pragma once

#include <engine/core/forward_declarations.hpp>

namespace sdl_engine
{
   class Scene
   {
   public:
      Scene( GameContext& ctx_ );
      virtual ~Scene() {};
      virtual void proc( f32 delta_ ) = 0;

      GameContext& getGameContext() { return _context; }

   private:
      GameContext& _context;
   };
}    // namespace sdl_engine