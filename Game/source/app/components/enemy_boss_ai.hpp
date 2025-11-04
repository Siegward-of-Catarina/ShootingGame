#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct EnemyBossAI
   {
      enum class Attack
      {
         Normal3,
         Normal5,
         Big1,
         Big2,
         Big3,
         LaserCharge,
         LaserCharging,
         LaserAttack,
         LaserEnd,
         Shooting,    // 発射中
         None,
      } attack;
      std::array<u32, 6> attack_weights;    // normal3, normal5, big1, big2, big3 laserに適応
      enum class Phase
      {
         Init,    // Phase1のweights設定フェーズ
         Phase1,
         Phase2,
         Phase3,
      } phase;
   };
}    // namespace myge