#pragma once
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class SystemInterface
   {
   public:
      SystemInterface( i32 priority_ );
      virtual ~SystemInterface();
      virtual void update( sdl_engine::GameContext& context_ ) = 0;
      i32          getPriority() const;

   private:
      i32 _priority {};
   };
}    // namespace sdl_engine
