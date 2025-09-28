#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class Vector2_f32
   {
   public:
      Vector2_f32( f32 d_0_ = 0.f, f32 d_1_ = 0.f );
      Vector2_f32( std::array<f32, 2>& array_ );

   public:
      std::array<f32, 2> data;

      f32& x { data[ 0 ] };
      f32& y { data[ 1 ] };
   };
   class Vector2_i32
   {
   public:
      Vector2_i32( i32 d_0_ = 0.f, i32 d_1_ = 0.f );
      Vector2_i32( std::array<i32, 2>& array_ );

   public:
      std::array<i32, 2> data;

      i32& x { data[ 0 ] };
      i32& y { data[ 1 ] };
   };
}    // namespace sdl_engine
