#include <app/components/highlightable.hpp>
#include <app/event/highlight_event.hpp>
#include <app/systems/highlight_system.hpp>
#include <engine/basic_component.hpp>
namespace myge
{
   HighlightSystem::HighlightSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& dispatcher_ )
     : SystemInterface { priority_, registry_ }, _dispatcher { dispatcher_ }, _is_changed { true }
   {
      _dispatcher.sink<HighlightEvent>().connect<&HighlightSystem::onHighlight>( this );
   }
   HighlightSystem::~HighlightSystem()
   {
      _dispatcher.sink<HighlightEvent>().disconnect<&HighlightSystem::onHighlight>( this );
   }
   void HighlightSystem::update( const sdl_engine::FrameData& frame_ )
   {
      if ( _is_changed )
      {
         _is_changed = false;
         for ( auto [ entt, select_txt, sprt ] : registry().view<Highlightable, sdl_engine::Sprite>().each() )
         {
            if ( select_txt.active ) { sprt.color = select_txt.active_color; }
            else { sprt.color = select_txt.inactive_color; }
         }
      }
   }
   void HighlightSystem::onHighlight( HighlightEvent& e )
   {
      for ( auto [ entt, select_txt ] : registry().view<Highlightable>().each() )
      {
         if ( entt == e.entity ) { select_txt.active = true; }
         else { select_txt.active = false; }
      }
      _is_changed = true;
   }
}    // namespace myge
