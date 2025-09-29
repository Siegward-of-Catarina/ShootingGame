#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class TransformUpdateSystem : public SystemInterface
   {
   public:
      TransformUpdateSystem();
      ~TransformUpdateSystem() override;
      // SystemInterface ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void update( entt::registry& registry_, GameContext& context_, f32 delta_time_ ) override;
      int  priority() const override;
   };
}    // namespace sdl_engine
