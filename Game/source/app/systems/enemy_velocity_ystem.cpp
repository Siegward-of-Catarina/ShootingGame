#include <app/systems/enemy_velocity_ystem.hpp>
#include <cmath>
#include <engine/Components/enemy_tag.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/velocity.hpp>
namespace myge
{
   EnemyVelocitySystem::EnemyVelocitySystem() {}

   EnemyVelocitySystem::~EnemyVelocitySystem() {}

   void EnemyVelocitySystem::update( entt::registry& registry_, f32 delta_time_ )
   {
      static float time = 0.0f;
      time += delta_time_ * 10;
      f32 target_x { 0 };
      f32 target_y { 0 };
      for ( auto [ entity ] : registry_.view<sdl_engine::EnemyTag>().each() )
      {
         auto& velocity  = registry_.get<sdl_engine::Velocity>( entity );
         auto& transform = registry_.get<sdl_engine::Transform>( entity );
         if ( target_x == 0 && target_y == 0 )
         {
            velocity.dx     = sinf( time ) * 200.f;    // 重力加速度を適用
            transform.angle = sinf( time ) * -30.0f + 180.0f;
         }
         else
         {
            velocity.dx = ( target_x - transform.x );
            velocity.dy = ( target_y - transform.y );
         }
         target_x = transform.x;
         target_y = transform.y;
      }
   }

}    // namespace myge
