#include <engine/Components/sprite_anim.hpp>

namespace sdl_engine {
    SpriteAnim createSpriteAnim(entt::resource<sdl_engine::SpriteAnimResource> resource_, const json& data_)
	{
        sdl_engine::SpriteAnim sprt_anim_comp{ .sprite_anim { resource_ },
                                               .current_frame { data_.value("current_frame", 0u) },
                                               .elapsed_time { 0.0 },
                                               .is_playing { data_.value("is_playing", true) } };
        return sprt_anim_comp;
	}
}