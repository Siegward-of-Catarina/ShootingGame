#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class EnemyVelocitySystem final : public sdl_engine::SystemInterface
   {
   public:
      EnemyVelocitySystem();
      ~EnemyVelocitySystem() override;

      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ ) override;
      int  priority() const override;
   };
}    // namespace myge
