#pragma once
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class SpriteRenderSystem : public SystemInterface
   {
   public:
      SpriteRenderSystem( i32 priority_ );
      virtual ~SpriteRenderSystem() override;
      // SystemInterface を介して継承されました
      virtual void  update( GameContext& context_ ) override;
   };
}    // namespace sdl_engine