#include <pch.hpp>
// my header
#include <engine/math/vector2.hpp>
namespace sdl_engine
{
   Vector2_f32::Vector2_f32( f32 x_, f32 y_ ) : x { x_ }, y { y_ } {}

   Vector2_f32::Vector2_f32( std::array<f32, 2>& array_ ) : x { array_[ 0 ] }, y { array_[ 1 ] } {}

   Vector2_i32        Vector2_f32::converti32() { return Vector2_i32( static_cast<i32>( x ), static_cast<i32>( y ) ); }
   std::array<f32, 2> Vector2_f32::array() { return std::array<f32, 2> { x, y }; }

   Vector2_f32& Vector2_f32::normalize()
   {
      auto len { length() };
      if ( len > 0 )
      {
         x = x / len;
         y = y / len;
      }
      return *this;
   }
   Vector2_f32 Vector2_f32::normalized()
   {
      auto len { length() };
      if ( len > 0 ) { return { x / len, y / len }; }
      return *this;
   }
   Vector2_f32& Vector2_f32::inverse()
   {
      x = -x;
      y = -y;
      return *this;
   }
   Vector2_f32 Vector2_f32::inversed() { return { -x, -y }; }
   float       Vector2_f32::lengthSq() { return x * x + y * y; }
   float       Vector2_f32::length() { return std::sqrt( lengthSq() ); }
   float       Vector2_f32::dot( const Vector2_f32& vec ) { return x * vec.x + y * vec.y; }

   Vector2_f32  Vector2_f32::operator*( const Vector2_f32& vec ) { return { x * vec.x, y * vec.y }; }
   Vector2_f32  Vector2_f32::operator*( const f32 value ) { return { x * value, y * value }; }
   Vector2_f32  Vector2_f32::operator/( const f32 value ) { return { x / value, y / value }; }
   Vector2_f32  Vector2_f32::operator+( const Vector2_f32& vec ) { return { x + vec.x, y + vec.y }; }
   Vector2_f32  Vector2_f32::operator-( const Vector2_f32& vec ) { return { x - vec.x, y - vec.y }; }
   Vector2_f32& Vector2_f32::operator+=( const Vector2_f32& vec )
   {
      x += vec.x;
      y += vec.y;
      return *this;
   }
   Vector2_f32& Vector2_f32::operator-=( const Vector2_f32& vec )
   {
      x -= vec.x;
      y -= vec.y;
      return *this;
   }
   Vector2_f32& Vector2_f32::operator*=( const f32 value )
   {
      x *= value;
      y *= value;
      return *this;
   }
   f32& Vector2_f32::operator[]( const i32 index ) { return index == 0 ? x : y; }
   Vector2_f32::operator bool() const { return x != 0.f || y != 0.f; }
}    // namespace sdl_engine

namespace sdl_engine
{
   Vector2_i32::Vector2_i32( i32 x_, i32 y_ ) : x { x_ }, y { y_ } {}

   Vector2_i32::Vector2_i32( std::array<i32, 2>& array_ ) : x { array_[ 0 ] }, y { array_[ 1 ] } {}

   Vector2_f32        Vector2_i32::convertf32() { return Vector2_f32( static_cast<f32>( x ), static_cast<f32>( y ) ); }
   std::array<i32, 2> Vector2_i32::array() { return std::array<i32, 2> { x, y }; }
   Vector2_i32        Vector2_i32::operator*( const Vector2_i32& vec ) { return { x * vec.x, y * vec.y }; }
   Vector2_i32        Vector2_i32::operator/( const i32 value ) { return { x / value, y / value }; }
   i32&               Vector2_i32::operator[]( const i32 index ) { return index == 0 ? x : y; }
}    // namespace sdl_engine
