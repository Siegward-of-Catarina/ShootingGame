#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
namespace myge
{
   enum class BulletType
   {
      Enemy_small,
      Enemy_Large,
      Player,
   };
   struct Shooter
   {
      f32                     cooldown {};
      f32                     wait {};        // 加算していくカウンタ
      i8                      num_shot {};    // 発射する弾数 -1で無限
      sdl_engine::Vector2_f32 bullet_direction {};
      f32                     speed {};
      BulletType              bullet_type { BulletType::Player };
   };

   struct ShootingEnemyTag
   {
   };
   struct FinishedShootTag
   {
   };
}    // namespace myge