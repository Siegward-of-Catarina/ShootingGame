#pragma once
#include <app/forward_declarations.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/waves/wave.hpp>
#include <engine/core/forward_declarations.hpp>
namespace myge
{
   class Wave1 final : public Wave
   {
   public:
      Wave1( WaveDependencies& dependencies_ );
      // Wave を介して継承されました
      virtual void start( entt::entity player_ = entt::null ) override;
      virtual void update( f32 delta_time_ ) override;

   private:
      // std::unique_ptr<sdl_engine::MovementSystem> _move_system;
      std::unique_ptr<EnemyMovementSystem> _enemy_system;
   };
}    // namespace myge
