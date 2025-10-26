#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>

namespace sdl_engine
{
   // 2D 速度/角速度/スケール変化
   struct Velocity
   {
      f32 dx, dy, anguler, scale_rate;
   };

   // JSON から Velocity を生成（キーは実装側に依存）
   Velocity createVelocity( const json& data_ );
}    // namespace sdl_engine