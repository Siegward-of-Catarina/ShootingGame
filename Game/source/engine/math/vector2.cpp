#include <engine/math/vector2.hpp>
namespace sdl_engine
{
   Vector2_f32::Vector2_f32( f32 d_0_, f32 d_1_ )
     : data {
        { d_0_, d_1_ }
   }
   {
   }

   Vector2_f32::Vector2_f32( std::array<f32, 2>& array_ ) : data { array_ } {}

   Vector2_i32::Vector2_i32( i32 d_0_, i32 d_1_ )
     : data {
        { d_0_, d_1_ }
   }
   {
   }

   Vector2_i32::Vector2_i32( std::array<i32, 2>& array_ ) : data { array_ } {}
}    // namespace sdl_engine
