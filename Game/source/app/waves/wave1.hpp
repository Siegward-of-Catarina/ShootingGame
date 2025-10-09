#pragma once
#include <app/forward_declarations.hpp>
#include <app/systems/enemy_velocity_ystem.hpp>
#include <app/waves/wave.hpp>
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/transfrom_update_system.hpp>
namespace myge
{
   class Wave1 final : public Wave
   {
   public:
      Wave1();
      // Wave を介して継承されました
      void start( sdl_engine::GameContext& context_ ) override;
      void update( sdl_engine::GameContext& context_ ) override;

   private:

   private:
      std::unique_ptr<sdl_engine::TransformUpdateSystem> _move_system;
      std::unique_ptr<EnemyVelocitySystem>               _enemy_system;
   };
}    // namespace myge
