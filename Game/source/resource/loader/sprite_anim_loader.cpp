
#include <resource/loader/sprite_anim_loader.hpp>
#include <resource/sprite_anim_resource.hpp>

namespace myge
{
	std::shared_ptr<SpriteAnimResource> SpriteAnimLoader::operator()(const json& data) const
	{

		SpriteAnimResource res;
		std::string        anim_name;
		u32                sprite_col;
		u32                sprite_row;

		// keyname‚ÆƒXƒvƒ‰ƒCƒg‚Ì—ñAs”‚ğæ“¾
		data.at("name").get_to(anim_name);
		data.at("sprite_col").get_to(sprite_col);
		data.at("sprite_row").get_to(sprite_row);
		// SpriteAnimResource‚Éî•ñ‚ğŠi”[
		data.at("sprite_name").get_to(res.sprite_name);
		data.at("frame_width").get_to(res.frame_width);
		data.at("frame_height").get_to(res.frame_height);
		data.at("frame_time").get_to(res.frame_time);
		data.at("is_loop").get_to(res.is_loop);

		// ƒtƒŒ[ƒ€”‚ğ‹‚ß‚ÄŠi”[
		res.frame_num = sprite_col * sprite_row;

		// ƒtƒŒ[ƒ€î•ñ‚ğ‹‚ß‚é‚ÄŠi”[
		res.frames.reserve(res.frame_num);
		res.frames.resize(res.frame_num);

		for (size_t i = 0; i < res.frame_num; i++)
		{
			res.frames[i].x = res.frame_width * (i % sprite_col);
			res.frames[i].y = res.frame_height * (i / sprite_col);
			res.frames[i].w = res.frame_width;
			res.frames[i].h = res.frame_height;
		}

		return std::make_shared<SpriteAnimResource>(res);
	}
}    // namespace myge