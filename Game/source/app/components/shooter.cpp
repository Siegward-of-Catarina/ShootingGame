#include <app/components/shooter.hpp>

namespace myge
{
   Shooter createShooter( const json& data_ )
   {
      Shooter shtr {
         3.0f, 0.0f, { 0.0f, -1.0f },
           BulletType::Player
      };

      if ( auto cooldown { sdl_engine::getJsonData<f32>( data_, "cooldown" ) }; cooldown )
      {
         shtr.cooldown = cooldown.value();
      }

      if ( auto bullet_velocity { sdl_engine::getJsonData<std::array<f32, 2>>( data_, "bullet_velocity" ) };
           bullet_velocity )
      {
         shtr.bullet_velocity = sdl_engine::Vector2_f32 { bullet_velocity.value() };
      }

      if ( auto bullet_type { sdl_engine::getJsonData<std::string>( data_, "bullet_type" ) }; bullet_type )
      {
         if ( bullet_type == "player" ) { shtr.bullet_type = BulletType::Player; }
         if ( bullet_type == "enemy_small" ) { shtr.bullet_type = BulletType::Enemy_small; }
         if ( bullet_type == "enemy_large" ) { shtr.bullet_type = BulletType::Enemy_Large; }
      }

      return shtr;
   }
}    // namespace myge