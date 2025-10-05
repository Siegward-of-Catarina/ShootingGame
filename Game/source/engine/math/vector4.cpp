
#include <engine/math/vector4.hpp>
namespace sdl_engine
{
	Vector4::Vector4(f32 d_0_, f32 d_1_, f32 d_2_, f32 d_3_)
		: data{ d_0_, d_1_, d_2_, d_3_ }
	{
	}
	Vector4::Vector4(std::array<f32, 4>& array_) : data{ array_ } {}
}    // namespace sdl_engine
