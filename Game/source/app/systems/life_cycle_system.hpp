#pragma once

#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   struct DeadEvent;

   class LifeCycleSystem final : public sdl_engine::SystemInterface
   {
   public:
      LifeCycleSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& dispatcher_ );
      virtual ~LifeCycleSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onEntityDead( DeadEvent& e );

   private:
      entt::dispatcher& _dispatcher;
   };
}    // namespace myge
