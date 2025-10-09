#pragma once
#include <engine/forward.hpp>
#include <engine/utils.hpp>
namespace myge {
	struct SpriteBrink {
		f32 speed{};
		f32 min_alpha{};
		enum class State {
			AddAlpha,
			SubAlpha
		}state{ State::SubAlpha };
	};

	SpriteBrink createSpriteBrink(const json& data_);
}