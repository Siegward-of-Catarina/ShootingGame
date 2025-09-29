#pragma once
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class SpriteAnimationSystem : public SystemInterface
   {
   public:
      SpriteAnimationSystem();
      virtual ~SpriteAnimationSystem() override;
      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void update( entt::registry& registry_, GameContext& context_, f32 delta_time_ ) override;
      int  priority() const override;
   };
}    // namespace sdl_engine
