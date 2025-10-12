#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class EnemyMovementSystem final : public sdl_engine::SystemInterface
   {
   public:
      EnemyMovementSystem( i32 priority_ );
      ~EnemyMovementSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge
