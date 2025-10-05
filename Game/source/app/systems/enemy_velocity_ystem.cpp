#include <app/components/enemy_tag.hpp>
#include <app/systems/enemy_velocity_ystem.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace myge
{
   EnemyVelocitySystem::EnemyVelocitySystem( i32 priority_ ) : SystemInterface { priority_ } {}

   EnemyVelocitySystem::~EnemyVelocitySystem() {}

   void EnemyVelocitySystem::update( sdl_engine::GameContext& context_ )
   {
      auto&        registry   = context_.getRegistry();
      auto         delta_time = context_.getGameTimer().getDeltaTime();
      static float time       = 0.0f;
      time += delta_time * 10;
      f32 target_x { 0 };
      f32 target_y { 0 };
      for ( auto [ entity ] : registry.view<EnemyTag, sdl_engine::Active>().each() )
      {
         auto& velocity  = registry.get<sdl_engine::Velocity>( entity );
         auto& transform = registry.get<sdl_engine::Transform>( entity );
         if ( target_x == 0 && target_y == 0 )
         {
            velocity.dx     = sinf( time ) * 200.f;
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
