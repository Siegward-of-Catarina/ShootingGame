#include <pch.hpp>
// my header
#include <app/systems/highlight_system.hpp>
// component
#include <app/components/highlightable.hpp>
#include <engine/basic_component.hpp>
// event
#include <app/events/highlight_event.hpp>
#include <engine/events/event_listener.hpp>

namespace myge
{
   HighlightSystem::HighlightSystem( i32                        priority_,
                                     entt::registry&            registry_,
                                     sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _is_changed { true }
   {

      _event_listener.connect<&HighlightSystem::onHighlight, HighlightEvent>( this );
   }
   HighlightSystem::~HighlightSystem() {}
   void HighlightSystem::update( const sdl_engine::FrameData& )
   {
      if ( _is_changed )
      {
         _is_changed = false;
         auto& reg { registry() };
         for ( auto [ entt, select_txt, sprt ] : registry().view<Highlightable, sdl_engine::Sprite>().each() )
         {
            if ( !reg.valid( entt ) ) { continue; }
            if ( select_txt.active ) { sprt.color = select_txt.active_color; }
            else { sprt.color = select_txt.inactive_color; }
         }
      }
   }
   void HighlightSystem::onHighlight( HighlightEvent& e )
   {
      auto& reg { registry() };
      for ( auto [ entity, select_txt ] : reg.view<Highlightable>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         if ( entity == e.entity ) { select_txt.active = true; }
         else { select_txt.active = false; }
      }
      _is_changed = true;
   }
}    // namespace myge
