#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{

   struct HitEvent;

   class HitResolutionSystem final : public sdl_engine::SystemInterface
   {
   public:
      HitResolutionSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~HitResolutionSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onHit( HitEvent& e );

   private:
      sdl_engine::EventListener& _event_listener;
   };
}    // namespace myge
