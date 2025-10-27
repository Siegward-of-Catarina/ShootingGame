#include <app/components/entity_type_tag.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/components/sin_wave_movement.hpp>
#include <app/systems/enemy_movement_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

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
      for ( auto [ entity, velo, movement ] :
            getLogicUpdateable<sdl_engine::Velocity, SerpentineMovement, EnemyTag>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         auto add_time = frame_.delta_time * movement.frequency;
         movement.time += add_time;
         // サイン波の現在値と1フレーム前の値を求める
         auto sinv     = sinf( movement.time );
         auto pre_sinv = sinf( movement.time - add_time );
         // サイン波のオフセット値を求め, 差分を計算
         auto target = sinv * movement.amplitude + movement.center_x;
         auto pre    = pre_sinv * movement.amplitude + movement.center_x;
         //_ x方向は横揺れ
         velo.dx = ( target - pre ) / ( frame_.delta_time );
         // y方向は端に行ったら降下
         velo.dy = ( abs( sinv ) > movement.move_threshold ) ? movement.move_speed : 0.f;
      }

      for ( auto [ entity, velo, movement ] :
            getLogicUpdateable<sdl_engine::Velocity, SinWaveMovement, EnemyTag>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }

         auto add_time = frame_.delta_time * movement.frequency;
         movement.time += add_time;

         // サイン波の現在値と1フレーム前の値を求める
         auto sinv     = sinf( movement.time );
         auto pre_sinv = sinf( movement.time - add_time );

         // サイン波のオフセット値を求め, 差分を計算
         auto offset      = sinv * movement.amplitude;
         auto pre_offset  = pre_sinv * movement.amplitude;
         auto diff_offset = offset - pre_offset;

         // 基本進行
         movement.direction.normalize();
         velo.dx = movement.direction.x * movement.move_speed;
         velo.dy = movement.direction.y * movement.move_speed;

         // 進行方向の垂直ベクトルを求める
         sdl_engine::Vector2_f32 normal { -movement.direction.y, movement.direction.x };

         // サイン波のオフセット分を加算
         velo.dx += normal.x * ( diff_offset / frame_.delta_time );
         velo.dy += normal.y * ( diff_offset / frame_.delta_time );
      }
   }

}    // namespace myge
