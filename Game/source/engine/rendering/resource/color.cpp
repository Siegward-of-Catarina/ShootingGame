#include <pch.hpp>
// my header
#include <engine/rendering/resource/color.hpp>

namespace sdl_engine
{

   ColorRGBA::ColorRGBA( f32 r_, f32 g_, f32 b_, f32 a_ ) : r { r_ }, g { g_ }, b { b_ }, a { a_ } {}
   ColorRGBA::ColorRGBA( std::array<f32, 4>& rgba_ )
     : r { rgba_[ 0 ] }, g { rgba_[ 1 ] }, b { rgba_[ 2 ] }, a { rgba_[ 3 ] }
   {
   }
}    // namespace sdl_engine
