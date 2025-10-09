#include <engine/Components/text.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/resource/font_resource.hpp>
namespace sdl_engine {
	Text createText(ResourceManager& resource_manager_, const json& data_)
	{
		auto font_name{ sdl_engine::getJsonData < std::string>(data_,"font").value() };
		auto font{ resource_manager_.getFont(font_name) };
		return Text{ font,data_.value("text", "") };
	}
}