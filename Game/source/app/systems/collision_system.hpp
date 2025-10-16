#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class CollisionSystem final : public sdl_engine::SystemInterface
   {
   public:
      CollisionSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& dispatcher_ );
      virtual ~CollisionSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      entt::dispatcher& _dispatcher;
   };
}    // namespace myge
