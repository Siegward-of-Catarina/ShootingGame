#pragma once

#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class LifeCycleSystem final : public sdl_engine::SystemInterface
   {
   public:
      LifeCycleSystem( i32 priority_, entt::registry& registry_ );
      virtual ~LifeCycleSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::EngineContext& context_ ) override;
   };
}    // namespace myge
