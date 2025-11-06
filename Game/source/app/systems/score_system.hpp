#pragma once
#include <app/events/player_life_changed_event.hpp>
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>

namespace myge
{
   struct DeadEvent;
   class ScoreSystem final : public sdl_engine::SystemInterface
   {
   public:
      ScoreSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~ScoreSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      // deadイベントを受け取ってスコアを加算する
      void onScoreChanged( DeadEvent& e );

   private:
      sdl_engine::EventListener& _event_listener;
   };
}    // namespace myge
