#include <pch.hpp>
// my header
#include <app/systems/screen_bounds_system.hpp>
// component
#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <engine/basic_component.hpp>
// core
#include <engine/core.hpp>
// utils
#include <engine/utils.hpp>
namespace
{
   struct BoxRect
   {
      f32 left;
      f32 right;
      f32 top;
      f32 bottom;
   };

   // 有効な方向のみで内側判定を行う
   bool isInside( const BoxRect& target_, const BoxRect& screen_, const myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      switch ( enable_axis_ )
      {
         case myge::BoundingBox::EnableAxis::Top : return target_.top >= screen_.top && target_.top <= screen_.bottom;
         case myge::BoundingBox::EnableAxis::Bottom :
            return target_.bottom >= screen_.top && target_.bottom <= screen_.bottom;
         case myge::BoundingBox::EnableAxis::Left :
            return target_.left >= screen_.left && target_.left <= screen_.right;
         case myge::BoundingBox::EnableAxis::Right :
            return target_.right >= screen_.left && target_.right <= screen_.right;
         case myge::BoundingBox::EnableAxis::LR : return target_.left >= screen_.left && target_.right <= screen_.right;
         case myge::BoundingBox::EnableAxis::TB : return target_.top >= screen_.top && target_.bottom <= screen_.bottom;
         case myge::BoundingBox::EnableAxis::ALL :
            return target_.top >= screen_.top && target_.bottom <= screen_.bottom && target_.left >= screen_.left
                && target_.right <= screen_.right;
      }
      return false;
   }
   // 有効方向が Top TB ALLの時に判定を行う
   bool isPartinalTop( f32 t_top_, f32 s_top_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Top or TB or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Top ) )
      {
         return t_top_ < s_top_;
      }
      return false;
   }
   // 有効方向が Bottom TB ALLの時に判定を行う
   bool isPartinalBottom( f32 t_bottom_, f32 s_bottom_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Bottom or TB or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Bottom ) )
      {
         return t_bottom_ > s_bottom_;
      }
      return false;
   }
   // 有効方向が Left LR ALLの時に判定を行う
   bool isPartinalLeft( f32 t_left_, f32 s_left_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Left or LR or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Left ) )
      {
         return t_left_ < s_left_;
      }
      return false;
   }
   // 有効方向が Right LR ALLの時に判定を行う
   bool isPartinalRight( f32 t_right_, f32 s_right_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Right or LR or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Right ) )
      {
         return t_right_ > s_right_;
      }
      return false;
   }
   // 有効方向が Bottom TB ALLの時に判定を行う
   bool isOutTop( f32 t_bottom_, f32 s_top_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Bottom or TB or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Bottom ) )
      {

         return t_bottom_ < s_top_;
      }
      return false;
   }
   // 有効方向が Top TB ALLの時に判定を行う
   bool isOutBottom( f32 t_top_, f32 s_bottom_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // TOP or TB or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Top ) )
      {
         return t_top_ > s_bottom_;
      }
      return false;
   }
   // 有効方向が Right LR ALLの時に判定を行う
   bool isOutLeft( f32 t_right_, f32 s_left_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Right or LR or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Right ) )
      {
         return t_right_ < s_left_;
      }
      return false;
   }
   // 有効方向が Left LR ALLの時に判定を行う
   bool isOutRight( f32 t_left_, f32 s_right_, myge::BoundingBox::EnableAxis& enable_axis_ )
   {
      // Left or LR or ALL
      if ( static_cast<u32>( enable_axis_ ) & static_cast<u32>( myge::BoundingBox::EnableAxis::Left ) )
      {
         return t_left_ > s_right_;
      }
      return false;
   }

}    // namespace
namespace myge
{
   ScreenBoundsSystem::ScreenBoundsSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface( priority_, registry_ )
   {
   }

   ScreenBoundsSystem::~ScreenBoundsSystem() {}

   void ScreenBoundsSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, box, trfm ] : reg.view<BoundingBox, sdl_engine::Transform>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }

         BoxRect target_bbox { .left { trfm.position.x - box.harf_width },
                               .right { trfm.position.x + box.harf_width },
                               .top { trfm.position.y - box.harf_hegiht },
                               .bottom { trfm.position.y + box.harf_hegiht } };

         BoxRect screen {
            .left { 0.f }, .right { frame_.window_width }, .top { 0.f }, .bottom { frame_.window_height }
         };
         // 画面外から登場するケースに対応するため、
         // ステートがNoneの場合Insideしか判定を取らない
         if ( box.state == BoundingBox::State::None )
         {
            // 有効方向[ enable_axis ]をもとに判定を行う
            if ( isInside( target_bbox, screen, box.enable_axis ) ) { box.state = BoundingBox::State::Inside; }
         }
         else
         {
            // 有効方向[ enable_axis ]をもとに判定を行う
            if ( isOutTop( target_bbox.bottom, screen.top, box.enable_axis ) )
            {
               box.state = BoundingBox::State::OutTop;
            }
            else if ( isOutBottom( target_bbox.top, screen.bottom, box.enable_axis ) )
            {
               box.state = BoundingBox::State::OutBottom;
            }
            else if ( isOutLeft( target_bbox.right, screen.left, box.enable_axis ) )
            {
               box.state = BoundingBox::State::OutLeft;
            }
            else if ( isOutRight( target_bbox.left, screen.right, box.enable_axis ) )
            {
               box.state = BoundingBox::State::OutRight;
            }
            else if ( isPartinalTop( target_bbox.top, screen.top, box.enable_axis ) )
            {
               box.state = BoundingBox::State::PartinalTop;
            }
            else if ( isPartinalBottom( target_bbox.bottom, screen.bottom, box.enable_axis ) )
            {
               box.state = BoundingBox::State::PartinalBottom;
            }
            else if ( isPartinalLeft( target_bbox.left, screen.left, box.enable_axis ) )
            {
               box.state = BoundingBox::State::PartinalLeft;
            }
            else if ( isPartinalRight( target_bbox.right, screen.right, box.enable_axis ) )
            {
               box.state = BoundingBox::State::PartinalRight;
            }
            else { box.state = BoundingBox::State::Inside; }
         }
      }
   }
}    // namespace myge
