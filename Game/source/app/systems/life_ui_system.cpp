// my header
#include <app/systems/life_ui_system.hpp>

// component
#include <app/components/damage.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/status.hpp>
#include <engine/basic_component.hpp>
// event
#include "score_system.hpp"

#include <app/event/player_life_changed_event.hpp>
#include <engine/events/event_listener.hpp>
namespace myge
{
   LifeUISystem::LifeUISystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
      _event_listener.connect<&LifeUISystem::onPlayerLifeChanged, PlayerLifeChangedEvent>( this );
   }
   LifeUISystem::~LifeUISystem() {}
   void LifeUISystem::update( const sdl_engine::FrameData& /*frame_*/ )
   {
      // 何もしない（イベントで更新）
   }
   void LifeUISystem::onPlayerLifeChanged( PlayerLifeChangedEvent& e )
   {
      auto& reg { registry() };
      for ( auto [ entity, repeat ] : getLogicUpdateable<sdl_engine::RepeatSprite, PlayerHPUITag>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         repeat.count = e.hp;
      }
   }
}    // namespace myge