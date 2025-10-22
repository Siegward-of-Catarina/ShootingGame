#pragma once
#include <engine/forward.hpp>
namespace myge {
	struct SpriteBrink {
		f32 speed{};
		f32 min_alpha{};
		enum class State {
			AddAlpha,
			SubAlpha
		}state{ State::SubAlpha };
	};
}