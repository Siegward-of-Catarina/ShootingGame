#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{

   class ScreenBoundsSystem final : public sdl_engine::SystemInterface
   {
   public:
      ScreenBoundsSystem( i32 priority_, entt::registry& registry_ );
      virtual ~ScreenBoundsSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::EngineContext& context_ ) override;
   };
}    // namespace myge
