#pragma once
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class ShootSystem final : public sdl_engine::SystemInterface
   {
   public:
      ShootSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& disp_ );
      virtual ~ShootSystem() override;
      // SystemInterface を介して継承されました
      virtual void update(const sdl_engine::FrameData& frame_) override;

   private:
      entt::dispatcher& _disp;
   };
}    // namespace myge