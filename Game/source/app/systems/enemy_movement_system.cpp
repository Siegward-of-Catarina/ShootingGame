#include <app/components/entity_type_tag.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace
{
   std::array<float, 4> time_ { 0.0f };
}    // namespace
namespace myge
{
   EnemyMovementSystem::EnemyMovementSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }

   EnemyMovementSystem::~EnemyMovementSystem() {}

   void EnemyMovementSystem::update( sdl_engine::EngineContext& context_ )
   {
      auto& registry { getRegistry() };
      auto  delta_time = context_.getGameTimer().getDeltaTime();
      for ( auto [ entity, trfm, velo, movement ] :
            getLogicUpdateable<sdl_engine::Transform, sdl_engine::Velocity, SerpentineMovement, EnemyTag>( registry )
              .each() )
      {
         auto add_time = delta_time * movement.frequency;
         movement.time += add_time;
         auto sinv     = sinf( movement.time );
         auto pre_sinv = sinf( movement.time - add_time );
         auto target   = sinv * movement.amplitude + movement.center_x;
         auto pre      = pre_sinv * movement.amplitude + movement.center_x;
         velo.dx       = ( target - pre ) / ( delta_time );

         velo.dy = ( abs( sinv ) > movement.move_threshold ) ? movement.move_speed : 0.f;
         // 後で分離
         trfm.angle = atan2( velo.dy, velo.dx ) * ( 180.0f / 3.141592f ) + 90;
      }
   }

}    // namespace myge
