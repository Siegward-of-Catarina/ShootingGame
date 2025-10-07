#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class Vector4
   {
   public:
      Vector4( f32 x_ = 0.0f, f32 y_ = 0.0f, f32 z_ = 0.0f, f32 w_ = 1.0f );
      Vector4( std::array<f32, 4>& data_ );
      f32& x() { return data[ 0 ]; }
      f32& y() { return data[ 1 ]; }
      f32& z() { return data[ 2 ]; }
      f32& w() { return data[ 3 ]; }

   public:
      std::array<f32, 4> data;
   };
}    // namespace sdl_engine
