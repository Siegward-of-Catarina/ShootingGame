#include <app/components/sprite_brink.hpp>
namespace myge {
	SpriteBrink createSpriteBrink(const json& data_)
	{

		auto speed{ sdl_engine::getOptionalData<f32>(data_,"brink_speed",1.0f) };
		auto min_alpha{ sdl_engine::getOptionalData<f32>(data_,"brink_min_alpha",0.3f) };
		SpriteBrink brink
		{
			.speed{speed},
			.min_alpha{min_alpha}
		};
		return brink;
	}
}