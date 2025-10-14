#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace
{

}
namespace myge
{
   OutOfScreenSystem::OutOfScreenSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }
   OutOfScreenSystem::~OutOfScreenSystem() {}
   void OutOfScreenSystem::update(const sdl_engine::FrameData& frame_)
   {
      auto& reg { registry() };

      auto player_view { getLogicUpdateable<BoundingBox, sdl_engine::Transform, sdl_engine::Velocity, PlayerTag>(
        reg ) };
      for ( auto [ entity, box, trfm, velo ] : player_view.each() )
      {
         if ( box.state == BoundingBox::State::PartinalTop )
         {
            velo.dy = 0;
            trfm.y  = static_cast<f32>( box.harf_hegiht );
         }
         if ( box.state == BoundingBox::State::PartinalBottom )
         {
            velo.dy = 0;
            trfm.y  = frame_.window_height - box.harf_hegiht;
         }
         if ( box.state == BoundingBox::State::PartinalLeft )
         {
            velo.dx = 0;
            trfm.x  = static_cast<f32>( box.harf_width );
         }
         if ( box.state == BoundingBox::State::PartinalRight )
         {
            velo.dx = 0;
            trfm.x  = frame_.window_width - box.harf_width;
         }
      }

      auto background_view {
         getLogicUpdateable<BoundingBox, sdl_engine::Transform, sdl_engine::Velocity, BackgroundTag>( reg )
      };
      for ( auto [ entity, box, trfm, velo ] : background_view.each() )
      {
         // screen2枚分戻す
         if ( box.state == BoundingBox::State::OutBottom ) { trfm.y -= frame_.window_height * 2; }
      }

      std::vector<entt::entity> destroy_entities {};
      auto                      enemy_view { getLogicUpdateable<BoundingBox, EnemyTag>( reg ) };

      for ( auto [ entity, box ] : enemy_view.each() )
      {
         if ( static_cast<u32>( box.state ) & static_cast<u32>( BoundingBox::State::Out ) )
         {
            destroy_entities.emplace_back( entity );
         }
      }

      // 消去対象のエンティティがあればDeadにする
      for ( auto& entity : destroy_entities ) { reg.emplace<DeadTag>( entity ); }
   }
}    // namespace myge
