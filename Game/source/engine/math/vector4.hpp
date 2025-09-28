#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class Vector4
   {
   public:
      Vector4( f32 d_0_ = 0.f, f32 d_1_ = 0.f, f32 d_2_ = 0.f, f32 d_3_ = 0.f );
      Vector4( std::array<f32, 4>& array_ );

   public:
      std::array<f32, 4> data;

      f32& x { data[ 0 ] };
      f32& y { data[ 1 ] };
      f32& z { data[ 2 ] };
      f32& w { data[ 3 ] };

      f32& r { data[ 0 ] };
      f32& g { data[ 1 ] };
      f32& b { data[ 2 ] };
      f32& a { data[ 3 ] };
   };
}    // namespace sdl_engine
