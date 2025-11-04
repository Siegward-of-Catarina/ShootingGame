#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math/vector2.hpp>
namespace sdl_engine
{
   struct RepeatSprite
   {
      enum class Type
      {
         Linear,
         Grid,
      } type;                       // 繰り返しタイプ
      u32         count;            // 繰り返し総数
      Vector2_f32 offset;           // 各スプライト間のオフセット
      u32         columns;          // Grid時の列数
      Vector2_f32 offset_column;    // 列ごとのオフセット（Grid時のみ）
   };
}    // namespace sdl_engine