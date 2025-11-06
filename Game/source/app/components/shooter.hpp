#pragma once
#include <engine/core/forward_declarations.hpp>
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
      f32                     wait {};              // 加算していくカウンタ
      i8                      num_shot {};          // 発射する弾数 -1で無限
      sdl_engine::Vector2_f32 spawn_position {};    // 弾丸の発射位置（相対座標）
      sdl_engine::Vector2_f32 bullet_direction {};
      f32                     speed {};
      i32                     bullet_hp {};
      u32                     bullet_atk {};
      BulletType              bullet_type { BulletType::Player };
   };
   struct MultipleShooter
   {
      std::vector<Shooter> shooters;
      u32                  current_index {};
   };
   struct LaserShooter
   {
      sdl_engine::Vector2_f32 spawn_position {};    // レーザーの発射位置（相対座標）
      f32                     speed {};
      sdl_engine::Vector2_f32 laser_direction {};
      f32                     laser_lifetime {};
      f32                     elapsed_time {};
      u32                     laser_atk {};
   };
   struct ShootingEnemyTag
   {
   };

   struct ShootingDoubleBulletEnemyTag
   {
   };
   struct ShootLaserEnemyTag
   {
   };
   struct FinishedShootTag
   {
   };
}    // namespace myge