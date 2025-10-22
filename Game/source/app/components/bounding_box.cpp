#include <app/components/bounding_box.hpp>

namespace myge
{
   BoundingBox createBoundingBox( f32 harf_width_, f32 harf_height_, f32 radius_, BoundingBox::EnableAxis enable_axis_ )
   {
      return BoundingBox { harf_width_, harf_height_, radius_, BoundingBox::State::None, enable_axis_ };
   }
}    // namespace myge