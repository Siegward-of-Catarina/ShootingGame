#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>

namespace sdl_engine
{
   // 2D Transform（位置/角度/スケール）
   struct Transform
   {
      f32 x;
      f32 y;
      f32 angle;
      f32 scale;
   };

   // JSON から Transform を生成（px/py/angle/scale）
   Transform createTransform( const json& data_ );

   // 既定 Transform（0,0,0,1）
   Transform createTransform();
}    // namespace sdl_engine