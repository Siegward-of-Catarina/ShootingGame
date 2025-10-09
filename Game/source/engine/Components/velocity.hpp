#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
	struct Velocity
	{
		f32 dx, dy, anguler, scale_rate;
	};

	Velocity createVelocity(const json& data_);

}    // namespace sdl_engine

