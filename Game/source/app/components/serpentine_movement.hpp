#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct SerpentineMovement
   {
      f32 center_x {};          // 蛇行の中心点
      f32 amplitude {};         // 蛇行幅
      f32 frequency {};         // timeに掛ける係数 デフォルト1.0f
      f32 move_speed {};        // 降下スピード
      f32 move_threshold {};    // 端での降下開始判定値 例) x > 0.9f ->降下
      // 内部用
      f32 time {};
   };

   // no factory functions here; constructed directly by callers
}    // namespace myge}    // namespace myge