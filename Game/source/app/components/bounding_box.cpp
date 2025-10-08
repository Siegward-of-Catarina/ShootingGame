#include <app/components/bounding_box.hpp>

namespace myge {
	BoundingBox createBoundingBox(const json& data_)
	{
		BoundingBox box_comp{ .harf_width { data_.value("harf_width", 0) },
							  .harf_hegiht { data_.value("harf_height",0) },
							  .radius { data_.value("radius",0.0f) },
							  .state { BoundingBox::State::None },    // ŒÅ’è
							  .enable_axis { BoundingBox::EnableAxis::ALL } };

		// —LŒø”»’è•ûŒü‚Éw’è‚ª‚ ‚ê‚ÎÄİ’è
		auto axis{ sdl_engine::getJsonData<std::string>(data_,"enable_axis") };
		if (axis)
		{
			// all‚Ìê‡‚·‚Å‚Éİ’è‚³‚ê‚Ä‚é‚Ì‚Å–³‹
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