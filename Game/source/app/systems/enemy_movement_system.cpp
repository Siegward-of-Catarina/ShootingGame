#include <app/components/entity_type_tag.hpp>
#include <app/components/serpentine_movement.hpp>
#include <app/components/shooter.hpp>
#include <app/components/sin_wave_movement.hpp>
#include <app/components/stop_and_shoot_movement.hpp>
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
      serpentineMovement( frame_ );
      sinWaveMovement( frame_ );
      stopAndShootMovement( frame_ );
   }

   void EnemyMovementSystem::serpentineMovement( const sdl_engine::FrameData& frame_ )
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
         velo.vector.x = ( target - pre ) / ( frame_.delta_time );
         // y方向は端に行ったら降下
         velo.vector.y = ( abs( sinv ) > movement.move_threshold ) ? movement.move_speed : 0.f;
      }
   }

   void EnemyMovementSystem::sinWaveMovement( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
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
         velo.vector = movement.direction * movement.move_speed;

         // 進行方向の垂直ベクトルを求める
         sdl_engine::Vector2_f32 normal { -movement.direction.y, movement.direction.x };

         // サイン波のオフセット分を加算
         velo.vector += normal * ( diff_offset / frame_.delta_time );
      }
   }

   void EnemyMovementSystem::stopAndShootMovement( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> change_tag_entity;
      for ( auto [ entity, trfm, velo, movement ] :
            getLogicUpdateable<sdl_engine::Transform, sdl_engine::Velocity, StopAndShootMovement, EnemyTag>( reg )
              .each() )
      {
         sdl_engine::Vector2_f32 to_target;
         switch ( movement.state )
         {
            case StopAndShootMovement::State::Entering :
               to_target = movement.stop_pos - trfm.position;
               to_target.normalize();
               velo.vector = to_target * movement.speed;
               // 到達判定
               if ( to_target.lengthSq() <= 0.02 )
               {
                  velo.vector    = { 0, 0 };
                  movement.state = StopAndShootMovement::State::Shooting;
               }
               break;
            case StopAndShootMovement::State::Shooting :
               // ShootingEnemyTagが付いていない = 射撃前 or　射撃終了後であるので付け替え対象に
               if ( !reg.all_of<ShootingEnemyTag>( entity ) ) { change_tag_entity.emplace_back( entity ); }
               break;
            case StopAndShootMovement::State::Exiting :
               to_target = movement.exit_pos - trfm.position;
               to_target.normalize();
               velo.vector = to_target * movement.speed;
               break;
         }
      }

      for ( auto& entity : change_tag_entity )
      {
         if ( reg.all_of<FinishedShootTag>( entity ) )
         {
            // 射撃終了後ならタグを外して退出状態へ
            reg.remove<ShootingEnemyTag>( entity );
            auto& movement = reg.get<StopAndShootMovement>( entity );
            movement.state = StopAndShootMovement::State::Exiting;
         }
         else
         {
            // 射撃前ならタグを付与して射撃開始
            reg.emplace<ShootingEnemyTag>( entity );
         }
      }
   }

}    // namespace myge
