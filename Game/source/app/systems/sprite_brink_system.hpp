#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class SpriteBrinkSystem final : public sdl_engine::SystemInterface
   {
   public:
      SpriteBrinkSystem( i32 priority_, entt::registry& registry_ );
      virtual ~SpriteBrinkSystem();
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::EngineContext& context_ ) override;
   };
}    // namespace myge
