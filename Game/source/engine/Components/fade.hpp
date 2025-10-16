#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{


	struct Fade
	{
		enum class State
		{
			FadeIn,
			FadeOut,
			BlackOut,
			FadeInEnd,
			FadeOutEnd,
			Idle,
		} state;
		enum class Type {
			OutIn,
			In,
			Out
		} type;
		f32 speed;
		f32 black_out_wait;
		f32 black_out_duration;
	};

	Fade createFade(const json& data_);
	Fade createFade();
}    // namespace sdl_engine