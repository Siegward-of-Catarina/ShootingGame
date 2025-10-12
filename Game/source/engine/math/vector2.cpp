#include <cmath>
#include <engine/math/vector2.hpp>
namespace sdl_engine
{
   Vector2_f32::Vector2_f32( f32 x_, f32 y_ ) : x { x_ }, y { y_ } {}

   Vector2_f32::Vector2_f32( std::array<f32, 2>& array_ ) : x { array_[ 0 ] }, y { array_[ 1 ] } {}

   Vector2_i32 Vector2_f32::converti32() { return Vector2_i32( static_cast<i32>( x ), static_cast<i32>( y ) ); }

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

   float Vector2_f32::lengthSq() { return x * x + y * y; }

   float Vector2_f32::length() { return std::sqrt( lengthSq() ); }
}    // namespace sdl_engine

namespace sdl_engine
{
   Vector2_i32::Vector2_i32( i32 x_, i32 y_ ) : x { x_ }, y { y_ } {}

   Vector2_i32::Vector2_i32( std::array<i32, 2>& array_ ) : x { array_[ 0 ] }, y { array_[ 1 ] } {}

   Vector2_f32 Vector2_i32::convertf32() { return Vector2_f32( static_cast<f32>( x ), static_cast<f32>( y ) ); }

}    // namespace sdl_engine
