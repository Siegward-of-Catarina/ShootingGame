#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{

   class CollisionSystem final : public sdl_engine::SystemInterface
   {
   public:
      CollisionSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~CollisionSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      sdl_engine::EventListener&                         _event_listener;
      std::vector<std::pair<entt::entity, entt::entity>> _hit_entity_pairs;
   };
}    // namespace myge
