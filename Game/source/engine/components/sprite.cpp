#include <engine/Components/sprite.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
namespace sdl_engine
{
	Sprite createSprite(ResourceManager& resource_manager_, const json& data_)
	{
		auto sprite_name{ sdl_engine::getRequireData<std::string>(data_, "name") };
		auto sprite = resource_manager_.getSprite(sprite_name);

		f32 tex_w{ static_cast<f32>(sprite->texture->w) };
		f32 tex_h{ static_cast<f32>(sprite->texture->h) };
		auto color = getOptionalData<std::array<f32, 4>>(data_, "color", { 1.0f,1.0f,1.0f,1.0f });
		// render orderはオプション。基本はレンダータイプごとにレイヤー分けされる。
		auto order{ getOptionalData<u32>(data_, "render_order",100) };

		sdl_engine::Sprite sprt_comp{
		   .texture { sprite },
		   .src { 0.f, 0.f, tex_w, tex_h },
		   .dst { 0.f, 0.f, tex_w, tex_h },
		   .color { color },
		   .render_order { order }
		};
		// color


		return sprt_comp;
	}
	Sprite createSprite(entt::resource<SpriteResource> sprite_resource_, ColorRGBA color_, u32 render_order_)
	{
		auto sprite = sprite_resource_;
		f32 tex_w{ static_cast<f32>(sprite->texture->w) };
		f32 tex_h{ static_cast<f32>(sprite->texture->h) };

		sdl_engine::Sprite sprt_comp{
		   .texture { sprite },
		   .src { 0.f, 0.f, tex_w, tex_h },
		   .dst { 0.f, 0.f, tex_w, tex_h },
		   .color { color_ },
		   .render_order { render_order_ }
		};

		return sprt_comp;
	}
}    // namespace sdl_engine