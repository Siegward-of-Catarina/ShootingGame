#include <app/components/sprite_brink.hpp>
namespace myge {
	SpriteBrink createSpriteBrink(const json& data_)
	{
		SpriteBrink brink
		{
			.speed{data_.value("speed",0.1f)},
			.min_alpha{data_.value("min_alpha",0.0f)}
		};
		return brink;
	}
}