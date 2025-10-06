#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{

   class ScreenBoundsSystem final : public sdl_engine::SystemInterface
   {
   public:
      ScreenBoundsSystem( i32 priority_ );
      virtual ~ScreenBoundsSystem() override;
      // SystemInterface ����Čp������܂���
      virtual void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge
