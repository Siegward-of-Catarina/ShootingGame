#include <engine/rendering/resource/color.hpp>

namespace sdl_engine
{

   ColorRGBA::ColorRGBA( f32 r_, f32 g_, f32 b_, f32 a_ ) : data { r_, g_, b_, a_ } {}
   ColorRGBA::ColorRGBA( std::array<f32, 4>& rgba_ ) : data { rgba_ } {}
}    // namespace sdl_engine
