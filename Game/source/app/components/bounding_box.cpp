#include <app/components/bounding_box.hpp>

namespace myge
{
   BoundingBox createBoundingBox( const json& data_ )
   {

       auto width{ sdl_engine::getRequireData<f32>(data_,"harf_width") };
       auto height{ sdl_engine::getRequireData<f32>(data_,"harf_height") };
       auto radius{ sdl_engine::getRequireData<f32>(data_,"radius") };
      BoundingBox box_comp { .harf_width {width },
                             .harf_hegiht { height },
                             .radius { radius },
                             .state { BoundingBox::State::None },    // 固定
                             .enable_axis { BoundingBox::EnableAxis::ALL } };

      // 有効判定方向に指定があれば再設定
      auto axis { sdl_engine::getOptionalData<std::string>( data_, "enable_axis","all")};
      // allの場合すでに設定されてるので無視
      if (axis != "all")
      {
          if (axis == "top") { box_comp.enable_axis = BoundingBox::EnableAxis::Top; }
          else if (axis == "bottom") { box_comp.enable_axis = BoundingBox::EnableAxis::Bottom; }
          else if (axis == "left") { box_comp.enable_axis = BoundingBox::EnableAxis::Left; }
          else if (axis == "right") { box_comp.enable_axis = BoundingBox::EnableAxis::Right; }
          else if (axis == "lr") { box_comp.enable_axis = BoundingBox::EnableAxis::LR; }
          else if (axis == "tb") { box_comp.enable_axis = BoundingBox::EnableAxis::TB; }
      }
      return box_comp;
   }
   BoundingBox createBoundingBox( f32 harf_width_, f32 harf_height_, f32 radius_, BoundingBox::EnableAxis enable_axis_ )
   {
      return BoundingBox { harf_width_, harf_height_, radius_, BoundingBox::State::None, enable_axis_ };
   }
}    // namespace myge