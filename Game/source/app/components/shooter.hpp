#pragma once
#include <engine/forward.hpp>
#include <engine/math.hpp>
#include <engine/utils.hpp>
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

   Shooter createShooter( const json& data_ );
   Shooter createShooter( f32 cooldown_, sdl_engine::Vector2_f32 dir, std::string_view type );
}    // namespace myge