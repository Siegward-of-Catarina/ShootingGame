#pragma once
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class SpriteAnimationSystem : public SystemInterface
   {
   public:
      SpriteAnimationSystem( i32 priority_, entt::registry& registry_ );
      virtual ~SpriteAnimationSystem() override;
      // SystemInterface を介して継承されました
      virtual void update(const FrameData& frame_) override;
   };
}    // namespace sdl_engine
