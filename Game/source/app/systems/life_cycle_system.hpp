#pragma once

#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class LifeCycleSystem final : public sdl_engine::SystemInterface
   {
   public:
      LifeCycleSystem( i32 priority_ );
      virtual ~LifeCycleSystem() override;
      // SystemInterface を介して継承されました
      virtual void  update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge
