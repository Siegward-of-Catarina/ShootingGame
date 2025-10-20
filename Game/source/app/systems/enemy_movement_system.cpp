#include <app/components/entity_type_tag.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

namespace
{
   std::array<f32, 4> time_ { 0.0f };
}    // namespace
namespace myge
{
   EnemyMovementSystem::EnemyMovementSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }

   EnemyMovementSystem::~EnemyMovementSystem() {}

   void EnemyMovementSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, trfm, velo, movement ] :
            getLogicUpdateable<sdl_engine::Transform, sdl_engine::Velocity, SerpentineMovement, EnemyTag>( reg )
              .each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         auto add_time = frame_.delta_time * movement.frequency;
         movement.time += add_time;
         auto sinv     = sinf( movement.time );
         auto pre_sinv = sinf( movement.time - add_time );
         auto target   = sinv * movement.amplitude + movement.center_x;
         auto pre      = pre_sinv * movement.amplitude + movement.center_x;
         velo.dx       = ( target - pre ) / ( frame_.delta_time );
         velo.dy       = ( abs( sinv ) > movement.move_threshold ) ? movement.move_speed : 0.f;
      }
   }

}    // namespace myge
