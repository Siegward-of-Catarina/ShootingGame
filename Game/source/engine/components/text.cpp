#include <engine/Components/text.hpp>
namespace sdl_engine {
	Text createText(const json& data_)
	{
		Text text_comp{ "",Text::Size::Small,{} };
		text_comp.text = data_.value("text", "");
		auto color{ getJsonData<std::array<f32,4>>(data_,"color") };
		if (color.has_value()) {
			text_comp.color = color.value();
		}
		auto size{ getJsonData<std::string>(data_,"size") };
		if ( size.has_value() ) {
			if (size.value() == "small") { text_comp.size = Text::Size::Small; }
			if (size.value() == "medium") { text_comp.size = Text::Size::Medium; }
			if (size.value() == "large") { text_comp.size = Text::Size::Large; }
		}
		return text_comp;
	}
}