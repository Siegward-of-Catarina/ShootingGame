#include <app/components/player_input.hpp>
#include <app/components/shooter.hpp>
#include <app/components/target.hpp>
#include <app/entity_factory.hpp>
#include <app/systems/shoot_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
//
#include <app/event/shoot_event.hpp>
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

         auto target_trfm         = reg.get<sdl_engine::Transform>( target.target_entity );
         shooter.bullet_direction = target_trfm.position - trfm.position;
         shooter.bullet_direction.normalize();

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
}    // namespace myge
