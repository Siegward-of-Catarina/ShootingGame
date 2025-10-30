#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   // 2D 速度/角速度/スケール変化
   struct Velocity
   {
      sdl_engine::Vector2_f32 vector;
      f32                     anguler, scale_rate;
   };

   // JSON から Velocity を生成（キーは実装側に依存）
   Velocity createVelocity( const json& data_ );
}    // namespace sdl_engine