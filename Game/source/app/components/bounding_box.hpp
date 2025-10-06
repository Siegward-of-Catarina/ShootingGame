#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct BoundingBox
   {
      i32 harf_width;
      i32 harf_hegiht;
      f32 radius;
      enum class State
      {
         Inside         = 0,
         Hit            = 1,
         PartinalTop    = 1 << 1,
         PartinalBottom = 1 << 2,
         PartinalLeft   = 1 << 3,
         PartinalRight  = 1 << 4,
         OutTop         = 1 << 5,
         OutBottom      = 1 << 6,
         OutLeft        = 1 << 7,
         OutRight       = 1 << 8,
         None           = 1 << 9,
         Out            = ( OutTop | OutBottom | OutLeft | OutRight ),
         Partinal       = ( PartinalTop | PartinalBottom | PartinalLeft | PartinalRight )
      } state;
      enum class EnableAxis
      {
         Top    = 1,
         Bottom = 1 << 1,
         Left   = 1 << 2,
         Right  = 1 << 3,
         TB     = ( Top | Bottom ),
         LR     = ( Left | Right ),
         ALL    = ( TB | LR )
      } enable_axis;
   };
}    // namespace myge
