#pragma once
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class SpriteRenderSystem : public SystemInterface
   {
   public:
      SpriteRenderSystem();
      virtual ~SpriteRenderSystem() override;
      // SystemInterface ����Čp������܂���
      void update( entt::registry& registry_, GameContext& context_, f32 delta_time_ ) override;
      int  priority() const override;
   };
}    // namespace sdl_engine