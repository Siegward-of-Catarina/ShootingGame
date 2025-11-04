#pragma once
#include <engine/forward.hpp>
namespace myge
{
   // 死亡エフェクト付与用コンポーネント
   struct DeadSE
   {
      std::string sound_key;
      f32         volume;    // 0.0f ～ 1.0f
   };
   struct DamageSE
   {
      std::string sound_key;
      f32         volume;    // 0.0f ～ 1.0f
   };
}    // namespace myge