#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class EnemyVelocitySystem final : public sdl_engine::SystemInterface
   {
   public:
      EnemyVelocitySystem( i32 priority_ );
      ~EnemyVelocitySystem() override;

      // SystemInterface ����Čp������܂���
      void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge
