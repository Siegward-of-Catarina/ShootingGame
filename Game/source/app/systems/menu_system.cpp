#include <pch.hpp>
// my header
#include <app/systems/menu_system.hpp>
// component
#include <app/components/button_ui.hpp>
#include <app/components/title_menu.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/events/highlight_event.hpp>
#include <app/events/key_down_event.hpp>
#include <app/events/menu_button_event.hpp>

namespace
{
   bool change = false;
}
namespace myge
{
   MenuSystem::MenuSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _down_space_key { false }
   {
      for ( auto [ entity, menu ] : registry().view<TitleMenu>().each() )
      {
         _event_listener.trigger<HighlightEvent>( { menu.menu_ui[ menu.selected ] } );
      }
      _event_listener.connect<&MenuSystem::onInputAction, KeyDownEvent>( this );
   }
   MenuSystem::~MenuSystem() {}
   void MenuSystem::update( const sdl_engine::FrameData& )
   {
      for ( auto [ entity, menu ] : registry().view<TitleMenu>().each() )
      {
         if ( change )
         {
            change = false;

            _event_listener.trigger<HighlightEvent>( { menu.menu_ui[ menu.selected ] } );
         }
         else if ( _down_space_key )
         {
            _down_space_key = false;

            auto& but = registry().get<ButtonUI>( menu.menu_ui[ menu.selected ] );
            _event_listener.trigger<MenuButtonEvent>( { but.type } );
         }
      }
   }
   void MenuSystem::onInputAction( KeyDownEvent& e )
   {
      for ( auto [ entity, menu ] : registry().view<TitleMenu>().each() )
      {
         if ( e.down_key == KeyDownEvent::EnableKeys::Down )
         {
            if ( menu.selected < menu.menu_ui.size() - 1 )
            {
               menu.selected++;
               change = true;
            }
         }
         if ( e.down_key == KeyDownEvent::EnableKeys::Up )
         {
            if ( menu.selected > 0 )
            {
               menu.selected--;
               change = true;
            }
         }
         if ( e.down_key == KeyDownEvent::EnableKeys::Space ) { _down_space_key = true; }
      }
   }
}    // namespace myge
