#pragma once
#include <engine/core/forward_declarations.hpp>
namespace myge
{
   // 射撃エフェクト付与用コンポーネント
   struct ShootSE
   {
      std::string sound_key;
      f32         volume {};    // 0.0f ～ 1.0f
   };
   // 複数射撃エフェクト付与用コンポーネント
   struct MultipleShootSE
   {
      // MultipleShooterのcurrent_indexに対応したShootSE配列
      std::vector<ShootSE> shoot_ses;
   };
   // 特殊射撃エフェクト付与用コンポーネント
   struct EXShootSE
   {
      std::string sound_key;
      f32         volume {};    // 0.0f ～ 1.0f
   };
   // 死亡エフェクト付与用コンポーネント
   struct DeadSE
   {
      std::string sound_key;
      f32         volume {};    // 0.0f ～ 1.0f
   };
   struct DamageSE
   {
      std::string sound_key;
      f32         volume {};    // 0.0f ～ 1.0f
   };
}    // namespace myge