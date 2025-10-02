#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class InputSystem final : public sdl_engine::SystemInterface
   {
   public:
      InputSystem( i32 priority_ );
      virtual ~InputSystem() override;
      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      virtual void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge