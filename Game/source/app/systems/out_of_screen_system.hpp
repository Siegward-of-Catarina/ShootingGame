#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class OutOfScreenSystem final : public sdl_engine::SystemInterface
   {
   public:
      OutOfScreenSystem();
      virtual ~OutOfScreenSystem() override;
      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      virtual void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ ) override;
      virtual int  priority() const override;
   };
}    // namespace myge