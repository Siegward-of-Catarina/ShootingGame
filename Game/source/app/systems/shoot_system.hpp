#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class ShootSystem final : public sdl_engine::SystemInterface
   {
   public:
      ShootSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~ShootSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void PlayerShoot( const sdl_engine::FrameData& frame_ );
      void EnemyBurstShoot( const sdl_engine::FrameData& frame_ );

   private:
      sdl_engine::EventListener& _event_listener;
   };
}    // namespace myge