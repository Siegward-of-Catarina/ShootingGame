#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
	class ColorRGBA
	{
	public:
		ColorRGBA(f32 r_ = 1.0f, f32 g_ = 1.0f, f32 b_ = 1.0f, f32 a_ = 1.0f);
		ColorRGBA(std::array<f32, 4>& rgba_);
		ColorRGBA        operator*(f32 val) { return { r * val,g * val,b * val,a * val }; }
		std::array<f32, 4> array() { return std::array<f32, 4> { r, g, b, a }; }

	public:
		f32 r;
		f32 g;
		f32 b;
		f32 a;
	};
}    // namespace sdl_engine
