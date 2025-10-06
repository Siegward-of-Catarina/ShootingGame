#include <app/components/bounding_box.hpp>
#include <app/components/lifecycle.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{
   LifeCycleSystem::LifeCycleSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   LifeCycleSystem::~LifeCycleSystem() {}
   void LifeCycleSystem::update( sdl_engine::GameContext& context_ )
   {
      //
      auto&                     registry { context_.getRegistry() };
      std::vector<entt::entity> change_tag_entities {};
      // 画面外で存在しているとき
      for ( auto [ entity ] : registry.view<EnteringTag>().each() )
      {
         // バウンディングボックスを持つオブジェクトの場合
         if ( registry.all_of<BoundingBox>( entity ) )
         {
            // 一度ゲームエリア内に入ったらアクティブ状態とする
            auto& box { registry.get<BoundingBox>( entity ) };
            if ( box.state == BoundingBox::State::Inside ) { change_tag_entities.emplace_back( entity ); }
         }
         else    // その他は出現時からアクティブに
         {
            change_tag_entities.emplace_back( entity );
         }
      }
      for ( auto [ entity ] : registry.view<ActiveTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DyingTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DeadTag>().each() ) {}

      for ( auto& entity : change_tag_entities )
      {
         if ( registry.all_of<EnteringTag>( entity ) )
         {

            registry.remove<EnteringTag>( entity );
            registry.emplace<ActiveTag>( entity );
            registry.emplace<sdl_engine::LogicUpdateableTag>( entity );
         }
      }
   }
}    // namespace myge
