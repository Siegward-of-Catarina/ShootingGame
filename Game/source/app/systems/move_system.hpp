#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class MoveSystem final : public sdl_engine::System
   {
   public:
      MoveSystem();
      ~MoveSystem() override;
      // System ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void update( entt::registry& registry_, f32 delta_time ) override;
   };
}    // namespace myge
