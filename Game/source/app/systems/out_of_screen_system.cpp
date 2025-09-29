#include <SDL3/SDL.h>
#include <app/components/bounding_box.hpp>
#include <app/components/out_of_screen_behavior.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/velocity.hpp>
#include <engine/core/game_context.hpp>
namespace myge
{
   OutOfScreenSystem::OutOfScreenSystem() {}
   OutOfScreenSystem::~OutOfScreenSystem() {}
   void OutOfScreenSystem::update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ )
   {
      for ( auto [ entity, behavior, box ] : registry_.view<OutOfScreenBehavior, BoundingBox>().each() )
      {
         auto& trfm        = registry_.get<sdl_engine::Transform>( entity );
         auto& velo        = registry_.get<sdl_engine::Velocity>( entity );
         auto& window_size = context_.getWindowSize();

         i32 left   = static_cast<i32>( box.harf_width );
         i32 right  = window_size.x + static_cast<i32>( box.harf_width );
         i32 top    = static_cast<i32>( box.harf_hegiht );
         i32 bottom = window_size.y + static_cast<i32>( box.harf_hegiht );

         // êÊÇ…è„â∫ç∂âEÇåvéZÇµÅAåãâ ÇÃÇ›Çï€éùÇµÇƒÇ®Ç≠
         bool is_out_of_top_or_btm { ( trfm.y <= top || trfm.y >= bottom ) ? true : false };
         bool is_out_of_lft_or_rht { ( trfm.x <= right || trfm.x >= left ) ? true : false };

         if ( is_out_of_top_or_btm || is_out_of_lft_or_rht )
         {
            switch ( behavior.type )
            {
               case OutOfScreenBeehaviorType::Destroy :
               {
                  registry_.destroy( entity );
                  break;
               }
               case OutOfScreenBeehaviorType::Wrap :
               {
                  if ( left > window_size.x ) { trfm.x = behavior.wrap_position_x; }
                  if ( top > window_size.y ) { trfm.y = behavior.wrap_position_y; }
                  break;
               }
               case OutOfScreenBeehaviorType::Stop :
               {
                  if ( is_out_of_top_or_btm ) { velo.dy = 0; }
                  if ( is_out_of_lft_or_rht ) { velo.dx = 0; }
                  break;
               }
                  auto y = trfm.y;
            }
         }
         // SDL_Log( "%f", trfm.y );
      }
   }
   int OutOfScreenSystem::priority() const { return 97; }
}    // namespace myge
