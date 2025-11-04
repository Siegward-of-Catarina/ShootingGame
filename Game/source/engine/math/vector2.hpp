#pragma once
#include <array>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class Vector2_f32
   {
   public:
      Vector2_f32( f32 d_0_ = 0.f, f32 d_1_ = 0.f );
      explicit Vector2_f32( std::array<f32, 2>& array_ );
      Vector2_i32        converti32();
      std::array<f32, 2> array();

      Vector2_f32& normalize();
      Vector2_f32  normalized();
      Vector2_f32& inverse();
      Vector2_f32  inversed();
      float        lengthSq();
      float        length();
      float        dot( const Vector2_f32& vec );

      Vector2_f32  operator*( const Vector2_f32& vec );
      Vector2_f32  operator*( const f32 value );
      Vector2_f32  operator/( const f32 value );
      Vector2_f32  operator+( const Vector2_f32& vec );
      Vector2_f32  operator-( const Vector2_f32& vec );
      Vector2_f32& operator+=( const Vector2_f32& vec );
      Vector2_f32& operator-=( const Vector2_f32& vec );
      Vector2_f32& operator*=( const f32 value );
      f32&         operator[]( const i32 index );
      explicit     operator bool() const;

   public:
      f32 x;
      f32 y;
   };

   class Vector2_i32
   {
   public:
      Vector2_i32( i32 d_0_ = 0.f, i32 d_1_ = 0.f );
      explicit Vector2_i32( std::array<i32, 2>& array_ );
      Vector2_f32        convertf32();
      std::array<i32, 2> array();
      Vector2_i32        operator*( const Vector2_i32& vec );
      Vector2_i32        operator/( const i32 value );
      i32&               operator[]( const i32 index );

   public:
      i32 x;
      i32 y;
   };

}    // namespace sdl_engine
