#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class InputSystem final : public sdl_engine::SystemInterface
   {
   public:
      InputSystem( i32 priority_, entt::registry& registry_, sdl_engine::InputManager& input_manager_ );
      virtual ~InputSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::EngineContext& context_ ) override;

   private:
      sdl_engine::InputManager& _input_manager;
   };
}    // namespace myge