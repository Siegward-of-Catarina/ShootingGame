#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   struct HighlightEvent;
   class HighlightSystem final : public sdl_engine::SystemInterface
   {
   public:
      HighlightSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~HighlightSystem() override;
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onHighlight( HighlightEvent& e );

   private:
      sdl_engine::EventListener& _event_listener;
      bool                       _is_changed;
   };
}    // namespace myge