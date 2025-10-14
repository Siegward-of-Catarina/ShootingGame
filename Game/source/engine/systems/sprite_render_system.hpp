#pragma once
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class SpriteRenderSystem : public SystemInterface
   {
   public:
      SpriteRenderSystem( i32 priority_, entt::registry& registry_, Renderer& renderer_ );
      virtual ~SpriteRenderSystem() override;
      // SystemInterface を介して継承されました
      virtual void update(const FrameData& frame_) override;

   private:
      Renderer& _renderer;
   };
}    // namespace sdl_engine