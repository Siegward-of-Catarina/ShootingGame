#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class EnemyMovementSystem final : public sdl_engine::SystemInterface
   {
   public:
      EnemyMovementSystem( i32 priority_, entt::registry& registry_ );
      ~EnemyMovementSystem() override;

      // SystemInterface を介して継承されました
      virtual void update(const sdl_engine::FrameData& frame_) override;
   };
}    // namespace myge
