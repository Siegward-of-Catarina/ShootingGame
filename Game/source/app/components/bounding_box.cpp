#include <app/components/bounding_box.hpp>

namespace myge {
	BoundingBox createBoundingBox(const json& data_)
	{
		BoundingBox box_comp{ .harf_width { data_.value("harf_width", 0) },
							  .harf_hegiht { data_.value("harf_height",0) },
							  .radius { data_.value("radius",0.0f) },
							  .state { BoundingBox::State::None },    // 固定
							  .enable_axis { BoundingBox::EnableAxis::ALL } };

		// 有効判定方向に指定があれば再設定
		auto axis{ sdl_engine::getJsonData<std::string>(data_,"enable_axis") };
		if (axis)
		{
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
		}
		return box_comp;
	}
}