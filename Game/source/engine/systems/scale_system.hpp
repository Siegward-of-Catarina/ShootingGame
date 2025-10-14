#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class ScaleSystem : public SystemInterface
   {
   public:
      ScaleSystem( i32 priority_, entt::registry& registry_ );
      virtual ~ScaleSystem() override;
      // SystemInterface を介して継承されました
      virtual void update(const FrameData& frame_) override;
   };
}    // namespace sdl_engine
