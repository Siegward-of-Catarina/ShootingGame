#pragma once
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class SystemInterface
   {
   public:
      virtual ~SystemInterface();
      virtual void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ = 0.0f ) = 0;
      virtual int  priority() const                                                                               = 0;
   };
}    // namespace sdl_engine
