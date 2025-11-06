#include <pch.hpp>
// my header
#include <app/systems/shoot_system.hpp>
// component
#include <app/components/player_input.hpp>
#include <app/components/shooter.hpp>
#include <app/components/target.hpp>
#include <engine/basic_component.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/events/shoot_event.hpp>
namespace myge
{

   ShootSystem::ShootSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
   }

   ShootSystem::~ShootSystem() {}

   void ShootSystem::update( const sdl_engine::FrameData& frame_ )
   {
      PlayerShoot( frame_ );
      EnemyBurstShoot( frame_ );
      EnemyDoubleBulletShoot( frame_ );
      EnemyLaserShoot( frame_ );
   }

   void ShootSystem::PlayerShoot( const sdl_engine::FrameData& frame_ )
   {
      for ( auto [ entity, shooter, input ] : getLogicUpdateable<Shooter, PlayerInput>( registry() ).each() )
      {
         if ( shooter.wait > shooter.cooldown && input.isShoot )
         {
            shooter.wait = 0.0f;
            _event_listener.trigger<ShootEvent>( { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += frame_.delta_time; }
      }
   }
   void ShootSystem::EnemyBurstShoot( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> finished_entity;
      for ( auto [ entity, shooter, trfm, velo, target ] :
            getLogicUpdateable<Shooter, sdl_engine::Transform, sdl_engine::Velocity, Target, ShootingEnemyTag>( reg )
              .each() )
      {
         if ( reg.valid( target.target_entity ) )
         {
            auto target_trfm         = reg.get<sdl_engine::Transform>( target.target_entity );
            shooter.bullet_direction = target_trfm.position - trfm.position;
            shooter.bullet_direction.normalize();
         }
         else
         {    // playerが死亡していたら真下に撃つ
            shooter.bullet_direction = { 0.0f, 1.0f };
         }

         if ( shooter.wait > shooter.cooldown )
         {
            shooter.num_shot--;
            shooter.wait = 0.0;

            _event_listener.trigger<ShootEvent>( { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += frame_.delta_time; }
         if ( shooter.num_shot == 0 ) { finished_entity.emplace_back( entity ); }
      }
      for ( auto entity : finished_entity )
      {
         // ShootingEnemyTag を外して FinishedBrastTag を付与
         reg.remove<ShootingEnemyTag>( entity );
         reg.emplace<FinishedShootTag>( entity );
      }
   }
   void ShootSystem::EnemyDoubleBulletShoot( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> finished_entity;
      for ( auto [ entity, shooter, trfm, velo ] :
            getLogicUpdateable<Shooter, sdl_engine::Transform, sdl_engine::Velocity, ShootingDoubleBulletEnemyTag>(
              reg )
              .each() )
      {

         if ( shooter.wait > shooter.cooldown )
         {
            shooter.num_shot--;
            shooter.wait = 0.0;

            _event_listener.trigger<ShootEvent>( { entity } );
            shooter.spawn_position.x = -shooter.spawn_position.x;    // 左右両方発射
            _event_listener.trigger<ShootEvent>( { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += frame_.delta_time; }

         if ( shooter.num_shot == 0 ) { finished_entity.emplace_back( entity ); }
      }
      for ( auto [ entity, mul_shooter, trfm, velo ] : getLogicUpdateable<MultipleShooter,
                                                                          sdl_engine::Transform,
                                                                          sdl_engine::Velocity,
                                                                          ShootingDoubleBulletEnemyTag>( reg )
                                                         .each() )
      {
         auto& shooter { mul_shooter.shooters[ mul_shooter.current_index ] };
         if ( shooter.wait > shooter.cooldown )
         {
            shooter.num_shot--;
            shooter.wait = 0.0;

            _event_listener.trigger<ShootEvent>( { entity } );
            shooter.spawn_position.x = -shooter.spawn_position.x;    // 左右両方発射
            _event_listener.trigger<ShootEvent>( { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += frame_.delta_time; }

         if ( shooter.num_shot == 0 ) { finished_entity.emplace_back( entity ); }
      }
      for ( auto entity : finished_entity )
      {
         // ShootingEnemyTag を外して FinishedBrastTag を付与
         reg.remove<ShootingDoubleBulletEnemyTag>( entity );
         reg.emplace<FinishedShootTag>( entity );
      }
   }
   void ShootSystem::EnemyLaserShoot( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> finished_entity;
      for ( auto [ entity, shooter ] : getLogicUpdateable<LaserShooter, ShootLaserEnemyTag>( reg ).each() )
      {
         if ( shooter.elapsed_time == 0.0f )
         {
            // 最初のフレームでレーザー発射イベントを発行
            _event_listener.trigger<LaserShootEvent>( { entity } );
            // 経過時間を進める
            shooter.elapsed_time += frame_.delta_time;
         }
         else
         {
            shooter.elapsed_time += frame_.delta_time;
            if ( shooter.elapsed_time >= shooter.laser_lifetime )
            {
               finished_entity.emplace_back( entity );
               shooter.elapsed_time = 0.0f;
            }
         }
      }

      for ( auto entity : finished_entity )
      {
         // ShootLaserEnemyTag を外して FinishedBrastTag を付与
         reg.remove<ShootLaserEnemyTag>( entity );
         reg.emplace<FinishedShootTag>( entity );
      }
   }
}    // namespace myge
