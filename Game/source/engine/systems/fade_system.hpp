#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class FadeSystem : public SystemInterface
   {
   public:
      FadeSystem( i32 priority_ );
      virtual ~FadeSystem() override;
      // SystemInterface を介して継承されました
      virtual void  update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace sdl_engine
