// my header
#include <app/systems/transform_link_system.hpp>

// component
#include <app/components/transform_link.hpp>
#include <engine/basic_component.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/event/dead_event.hpp>

namespace
{
   constexpr size_t DEFAULT_RESERVE_SIZE { 2 };
}

namespace myge
{
   // 子エンティティの Transform を親に追従させるシステム
   // 親が無効化された場合、子を DeadEvent で破棄させる
   TransformLinkSystem::TransformLinkSystem( i32                        priority_,
                                             entt::registry&            registry_,
                                             sdl_engine::EventListener& event_listener_ )
     : sdl_engine::SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _dead_link_entities {}
   {
      _dead_link_entities.reserve( DEFAULT_RESERVE_SIZE );
   }

   TransformLinkSystem::~TransformLinkSystem() {}

   void TransformLinkSystem::update( const sdl_engine::FrameData& /*frame_*/ )
   {
      auto& reg { registry() };
      _dead_link_entities.clear();

      for ( auto [ entity, trfm, link ] : reg.view<sdl_engine::Transform, TransformLink>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         if ( !reg.valid( link.parent ) )
         {
            // 親が無効になっていたら死
            _dead_link_entities.emplace_back( entity );
            continue;
         }
         // 親の Transform を参照して位置を更新
         auto trfm_parent = reg.get<sdl_engine::Transform>( link.parent );
         trfm.position.x  = trfm_parent.position.x + link.offset_pos.x;
         trfm.position.y  = trfm_parent.position.y + link.offset_pos.y;
      }

      if ( !_dead_link_entities.empty() ) { _event_listener.trigger<DeadEvent>( { _dead_link_entities } ); }
   }
}    // namespace myge