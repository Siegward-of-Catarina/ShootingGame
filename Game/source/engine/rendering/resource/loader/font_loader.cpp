#include <engine/rendering/resource/loader/font_loader.hpp>
#include <engine/rendering/resource/font_resource.hpp>
namespace sdl_engine {
	FontLoader::result_type FontLoader::operator()(const json& data_) const
	{
		FontResource res
		{
			.width{getJsonData<f32>(data_,"width").value()},
			.height{getJsonData<f32>(data_,"height").value()}
		};

		u32 sprite_row{ getJsonData<u32>(data_,"sprite_row").value() };
		u32 sprite_col{ getJsonData<u32>(data_,"sprite_col").value() };
		u32 font_num{ sprite_row * sprite_col };

		res.font_rect.reserve(font_num);
		res.font_rect.resize(font_num);

		for (u32 i{ 0 }; i < font_num; i++) {
			res.font_rect[i].x = static_cast<f32>(res.width * (i % sprite_col));
			res.font_rect[i].y = static_cast<f32>(res.height * (i / sprite_col));
			res.font_rect[i].w = static_cast<f32>(res.width);
			res.font_rect[i].h = static_cast<f32>(res.height);
		}

		return std::make_shared<FontResource>(res);
	}
}