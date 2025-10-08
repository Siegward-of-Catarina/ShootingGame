#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class ColorRGBA
   {
   public:
      ColorRGBA( f32 r_ = 1.0f, f32 g_ = 1.0f, f32 b_ = 1.0f, f32 a_ = 1.0f );
      ColorRGBA( std::array<f32, 4>& rgba_ );

      f32& r() { return data[ 0 ]; }
      f32& g() { return data[ 1 ]; }
      f32& b() { return data[ 2 ]; }
      f32& a() { return data[ 3 ]; }

   public:
      std::array<f32, 4> data;
   };
}    // namespace sdl_engine
