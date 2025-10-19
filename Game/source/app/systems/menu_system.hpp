#pragma once

#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   struct KeyDownEvent;

   class MenuSystem final : public sdl_engine::SystemInterface
   {
   public:
      MenuSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~MenuSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onInputAction( KeyDownEvent& e );

   private:
      sdl_engine::EventListener& _event_listener;
      bool                       _down_space_key;
   };
}    // namespace myge
