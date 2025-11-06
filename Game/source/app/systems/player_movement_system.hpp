#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class PlayerMovementSystem final : public sdl_engine::SystemInterface
   {
   public:
      PlayerMovementSystem( i32 priority_, entt::registry& registry_ );
      virtual ~PlayerMovementSystem() override;

      // SystemInterface を介して継承されました
      virtual void update(const sdl_engine::FrameData& frame_) override;
   };
}    // namespace myge
