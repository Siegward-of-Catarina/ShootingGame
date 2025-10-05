#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
	class Vector4
	{
	public:
		Vector4(f32 d_0_ = 0.0f, f32 d_1_ = 0.0f, f32 d_2_ = 0.0f, f32 d_3_ = 1.0f);
		Vector4(std::array<f32, 4>& array_);
	public:
		union {
			struct { f32 x, y, z, w; };
			struct { f32 r, g, b, a; };
			std::array<f32, 4> data;
		};
	};
}    // namespace sdl_engine
