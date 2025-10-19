#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   struct FadeOutStartEvent;
   class FadeSystem : public SystemInterface
   {
   public:
      FadeSystem( i32 priority_, entt::registry& registry_, EventListener& event_listener_ );
      virtual ~FadeSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const FrameData& frame_ ) override;

   private:
      void onFadeOutStart( FadeOutStartEvent& e );

   private:
      EventListener&                   _event_listener;
      std::unordered_set<entt::entity> _fades;
   };
}    // namespace sdl_engine
