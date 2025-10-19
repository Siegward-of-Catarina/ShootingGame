#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class InputSystem final : public sdl_engine::SystemInterface
   {
   public:
      InputSystem( i32                        priority_,
                   entt::registry&            registry_,
                   sdl_engine::EventListener& event_listener_,
                   sdl_engine::InputManager&  input_manager_ );
      virtual ~InputSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      sdl_engine::InputManager&  _input_manager;
      sdl_engine::EventListener& _event_listener;
   };
}    // namespace myge