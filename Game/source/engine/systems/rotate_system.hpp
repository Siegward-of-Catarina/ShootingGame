#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>

namespace sdl_engine
{
   class RotateSystem : public SystemInterface
   {
   public:
      RotateSystem( i32 priority_, entt::registry& registry_ );
      virtual ~RotateSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( EngineContext& context_ ) override;
   };
}    // namespace sdl_engine
