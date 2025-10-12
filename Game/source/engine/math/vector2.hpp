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
      Vector2_i32        converti32();
      Vector2_f32        operator*( Vector2_f32& vec ) { return { x * vec.x, y * vec.y }; }
      Vector2_f32        operator/( f32 value ) { return { x / value, y / value }; }
      f32&               operator[]( i32 index ) { return index == 0 ? x : y; }
      std::array<f32, 2> array() { return std::array<f32, 2> { x, y }; }

      Vector2_f32& normalize();
      Vector2_f32  normalized();
      float        lengthSq();
      float        length();

   public:
      f32 x;
      f32 y;
   };

   class Vector2_i32
   {
   public:
      Vector2_i32( i32 d_0_ = 0.f, i32 d_1_ = 0.f );
      Vector2_i32( std::array<i32, 2>& array_ );
      Vector2_f32        convertf32();
      Vector2_i32        operator*( Vector2_i32& vec ) { return { x * vec.x, y * vec.y }; }
      Vector2_i32        operator/( i32 value ) { return { x / value, y / value }; }
      i32&               operator[]( i32 index ) { return index == 0 ? x : y; }
      std::array<i32, 2> array() { return std::array<i32, 2> { x, y }; }

   public:
      i32 x;
      i32 y;
   };

}    // namespace sdl_engine
