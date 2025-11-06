#pragma once
#include <engine/core/forward_declarations.hpp>
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
      sdl_engine::EventListener& _event_listener;
      // 衝突したエンティティペア
      // 順番を固定する。first:はエネミーサイド、second:はプレイヤーサイド
      std::vector<std::pair<entt::entity, entt::entity>> _hit_entity_pairs;
   };
}    // namespace myge
