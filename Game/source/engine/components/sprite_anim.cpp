#include <engine/Components/sprite_anim.hpp>
#include <engine/managers/resource_manager.hpp>
namespace sdl_engine {
	SpriteAnim createSpriteAnim(ResourceManager& resource_manager_, const json& data_)
	{
		// SpriterAnimResoruce取得
		auto sprite_anim_name{ sdl_engine::getJsonData<std::string>(data_, "name") };
		auto sprt_anim_data{ resource_manager_.getSpriteAnim(sprite_anim_name.value()) };

		sdl_engine::SpriteAnim sprt_anim_comp{ .sprite_anim { sprt_anim_data },
											   .current_frame { data_.value("current_frame", 0u) },
											   .elapsed_time { 0.0 },
											   .is_playing { data_.value("is_playing", true) } };
		return sprt_anim_comp;
	}
}