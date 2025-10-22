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
      f32                     wait {};
      sdl_engine::Vector2_f32 bullet_velocity {};
      BulletType              bullet_type { BulletType::Player };
   };

}    // namespace myge