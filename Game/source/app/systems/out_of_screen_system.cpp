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
      auto& registry = context_.getRegistry();
      for ( auto [ entity, behavior, box, trfm, velo ] :
            registry.view<OutOfScreenBehavior, BoundingBox, sdl_engine::Transform, sdl_engine::Velocity>().each() )
      {
         // target box
         f32 t_left   = trfm.x - box.harf_width;
         f32 t_right  = trfm.x + box.harf_width;
         f32 t_top    = trfm.y - box.harf_hegiht;
         f32 t_bottom = trfm.y + box.harf_hegiht;
         // screen box
         f32 s_left   = 0.f;
         f32 s_right  = static_cast<f32>( context_.getWindowSize().x );
         f32 s_top    = 0.f;
         f32 s_bottom = static_cast<f32>( context_.getWindowSize().y );

         // 先に上下左右を計算し、結果のみを保持しておく

         // バウンディングボックスが少しでもはみ出す場合
         bool is_out_t_top_at_s_top { ( t_top < s_top ) ? true : false };
         bool is_out_t_bottom_at_s_bottom { ( t_bottom > s_bottom ) ? true : false };
         bool is_out_t_left_at_s_left { ( t_left < s_left ) ? true : false };
         bool is_out_t_right_at_s_right { ( t_right > s_right ) ? true : false };
         // バウンディングボックス全体がはみ出す場合
         bool is_out_t_bottom_at_s_top { ( t_bottom < s_top ) ? true : false };
         bool is_out_t_top_at_s_bottom { ( t_top > s_bottom ) ? true : false };
         bool is_out_t_right_at_s_left { ( t_right < s_left ) ? true : false };
         bool is_out_t_left_at_s_right { ( t_left > s_right ) ? true : false };

         switch ( behavior.type )
         {
            case OutOfScreenBehavior::Type::Destroy :
            {
               if ( is_out_t_bottom_at_s_top || is_out_t_top_at_s_bottom || is_out_t_right_at_s_left
                    || is_out_t_left_at_s_right )
               {
                  registry.destroy( entity );
                  continue;
               }
               break;
            }
            case OutOfScreenBehavior::Type::Wrap :
            {
               if ( is_out_t_bottom_at_s_top ) { trfm.y = s_bottom + box.harf_hegiht; }
               if ( is_out_t_top_at_s_bottom ) { trfm.y = static_cast<f32>( -box.harf_hegiht ); }
               if ( is_out_t_right_at_s_left ) { trfm.x = s_right + box.harf_width; }
               if ( is_out_t_left_at_s_right ) { trfm.x = static_cast<f32>( -box.harf_width ); }
               break;
            }
            case OutOfScreenBehavior::Type::Stop :
            {
               if ( is_out_t_top_at_s_top ) { trfm.y = static_cast<f32>( box.harf_hegiht ); }
               if ( is_out_t_bottom_at_s_bottom ) { trfm.y = s_bottom - box.harf_hegiht; }
               if ( is_out_t_left_at_s_left ) { trfm.x = static_cast<f32>( box.harf_width ); }
               if ( is_out_t_right_at_s_right ) { trfm.x = s_right - box.harf_width; }
               break;
            }
            case OutOfScreenBehavior::Type::Ignore :
            {
               if ( !is_out_t_bottom_at_s_top && !is_out_t_top_at_s_bottom && !is_out_t_right_at_s_left
                    && !is_out_t_left_at_s_right )
               {
                  behavior.type = OutOfScreenBehavior::Type::Destroy();
               }
               break;
            }
         }
      }
   }
}    // namespace myge
