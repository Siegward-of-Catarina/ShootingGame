#include <engine/Components/fade.hpp>

namespace sdl_engine {

	Fade createFade(const json& data_)
	{
		Fade fade_comp{ Fade::State::Idel };
		fade_comp.speed = data_.value("speed", 2.0f);
		fade_comp.black_out_time = data_.value("black_out_time", 1.0f);
		return fade_comp;
	}
	Fade createFade()
	{
		return Fade{ Fade::State::Idel,2.0f,1.0f };
	}
}