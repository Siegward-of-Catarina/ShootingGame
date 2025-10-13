#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class FadeSystem : public SystemInterface
   {
   public:
      FadeSystem( i32 priority_, entt::registry& registry_ );
      virtual ~FadeSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::EngineContext& context_ ) override;
   };
}    // namespace sdl_engine
