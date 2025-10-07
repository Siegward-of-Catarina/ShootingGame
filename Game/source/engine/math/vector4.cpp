
#include <engine/math/vector4.hpp>
namespace sdl_engine
{
   Vector4::Vector4( f32 x_, f32 y_, f32 z_, f32 w_ ) : data { x_, y_, z_, w_ } {}
   Vector4::Vector4( std::array<f32, 4>& data_ ) : data { data_ } {}
}    // namespace sdl_engine
