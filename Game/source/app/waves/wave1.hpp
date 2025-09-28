#pragma once
#include <app/forward_declarations.hpp>
#include <app/systems/enemy_velocity_ystem.hpp>
#include <app/systems/move_system.hpp>
#include <app/waves/wave.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class Wave1 final : public Wave
   {
   public:
      Wave1();
      // Wave ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void start( entt::registry& registry_, sdl_engine::GameContext& context_ ) override;
      void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ ) override;

   private:
      void createEnemys( entt::registry& registry_, sdl_engine::GameContext& context_ );

   private:
      std::unique_ptr<MoveSystem>          _move_system;
      std::unique_ptr<EnemyVelocitySystem> _enemy_system;
   };
}    // namespace myge
