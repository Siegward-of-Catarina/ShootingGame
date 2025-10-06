#include <app/components/bounding_box.hpp>
#include <app/components/out_of_screen_behavior.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace myge
{
   OutOfScreenSystem::OutOfScreenSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   OutOfScreenSystem::~OutOfScreenSystem() {}
   void OutOfScreenSystem::update( sdl_engine::GameContext& context_ )
   {
      auto& registry { context_.getRegistry() };
      auto& window_size { context_.getWindowSize() };
      f32   w_right  = static_cast<f32>( window_size.x );
      f32   w_bottom = static_cast<f32>( window_size.y );
      auto  view { getLogicUpdateable<OutOfScreenBehavior, BoundingBox, sdl_engine::Transform, sdl_engine::Velocity>(
        registry ) };
      for ( auto [ entity, behavior, box, trfm, velo ] : view.each() )
      {
         // Ç±Ç±ÇÕentityÇÃÉ^ÉOÇ≤Ç∆Ç…èàóùÇåàÇﬂÇÈÇÊÇ§Ç…Ç∑ÇÈ
         // player or enemy or bg...
         switch ( behavior.type )
         {
            case OutOfScreenBehavior::Type::Destroy :
            {
               if ( static_cast<u32>( box.state ) & static_cast<u32>( BoundingBox::State::Out ) )
               {
                  registry.destroy( entity );
                  continue;
               }
               break;
            }
            case OutOfScreenBehavior::Type::Wrap :
            {
               if ( box.state == BoundingBox::State::OutTop ) { trfm.y = w_bottom + box.harf_hegiht; }
               if ( box.state == BoundingBox::State::OutBottom ) { trfm.y = static_cast<f32>( -box.harf_hegiht ); }
               if ( box.state == BoundingBox::State::OutLeft ) { trfm.x = w_right + box.harf_width; }
               if ( box.state == BoundingBox::State::OutRight ) { trfm.x = static_cast<f32>( -box.harf_width ); }
               break;
            }
            case OutOfScreenBehavior::Type::Stop :
            {
               if ( box.state == BoundingBox::State::PartinalTop )
               {
                  velo.dy = 0;
                  trfm.y  = static_cast<f32>( box.harf_hegiht );
               }
               if ( box.state == BoundingBox::State::PartinalBottom )
               {
                  velo.dy = 0;
                  trfm.y  = w_bottom - box.harf_hegiht;
               }
               if ( box.state == BoundingBox::State::PartinalLeft )
               {
                  velo.dx = 0;
                  trfm.x  = static_cast<f32>( box.harf_width );
               }
               if ( box.state == BoundingBox::State::PartinalRight )
               {
                  velo.dx = 0;
                  trfm.x  = w_right - box.harf_width;
               }
               break;
            }
         }
      }
   }
}    // namespace myge
