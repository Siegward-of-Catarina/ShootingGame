#pragma once
#include <app/forward_declarations.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <app/waves/wave.hpp>
#include <engine/core/forward_declarations.hpp>
namespace myge
{
   class BasicWave final : public Wave
   {
   public:
      BasicWave( WaveDependencies& dependencies_ );
      // Wave を介して継承されました
      virtual void start( entt::entity player_ = entt::null ) override;
      virtual void update( f32 delta_time_ ) override;

   private:
   };
}    // namespace myge
