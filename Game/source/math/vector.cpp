
#include "vector.hpp"
namespace myge {

	Vec4::Vec4(f32 d_0_, f32 d_1_, f32 d_2_, f32 d_3_)
		: data{ { d_0_, d_1_, d_2_, d_3_ } }
	{}
	Vec4::Vec4(std::array<f32, 4>& array_)
		: data{ array_ }
	{
	}
}

