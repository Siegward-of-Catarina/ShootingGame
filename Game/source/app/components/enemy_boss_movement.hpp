#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct EnemyBossMovement
   {
      f32 amplitude {};    // 振幅
      f32 frequency {};    // 周波数
      f32 stop_py {};      // 停止位置Y座標
      f32 time {};         // 経過時間
   };

}    // namespace myge