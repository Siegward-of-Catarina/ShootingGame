#include <engine/Components/transform.hpp>

namespace sdl_engine {
	Transform createTransform(const json& data_)
	{
		Transform trfm_comp{ .x { data_.value("px",0.0f) },
							 .y {  data_.value("py",0.0f) },
							 .angle {  data_.value("angle",0.0f) },
							 .scale {  data_.value("scale",1.0f) } };
		return trfm_comp;
	}
}