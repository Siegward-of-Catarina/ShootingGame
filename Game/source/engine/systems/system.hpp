#pragma once
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class System
   {
   public:
      System();
      virtual ~System();
      virtual void update( entt::registry& registry_, f32 delta_ ) = 0;
   };
}    // namespace sdl_engine
