#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class OutOfScreenSystem final : public sdl_engine::SystemInterface
   {
   public:
      OutOfScreenSystem( i32 priority_ );
      virtual ~OutOfScreenSystem() override;
      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      virtual void  update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge