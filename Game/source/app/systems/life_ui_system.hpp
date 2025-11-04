#pragma once
#include <app/event/player_life_changed_event.hpp>
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>

namespace myge
{

   class LifeUISystem final : public sdl_engine::SystemInterface
   {
   public:
      LifeUISystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~LifeUISystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onPlayerLifeChanged( PlayerLifeChangedEvent& e );

   private:
      sdl_engine::EventListener& _event_listener;
   };
}    // namespace myge
