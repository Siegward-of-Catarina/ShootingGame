#include <app/components/player_input.hpp>
#include <app/components/shooter.hpp>
#include <app/entity_factory.hpp>
#include <app/systems/shoot_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{

   ShootSystem::ShootSystem( i32 priority_ ) : SystemInterface { priority_ } {}

   ShootSystem::~ShootSystem() {}

   void ShootSystem::update( sdl_engine::GameContext& context_ )
   {
      auto& registry { context_.getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };
      for ( auto [ entity, shooter, input ] : getLogicUpdateable<Shooter, PlayerInput>( registry ).each() )
      {
         if ( shooter.wait > shooter.cooldown && input.isShoot )
         {
            shooter.wait = 0.0f;
            EntityFactory factory;
            factory.createBullet( context_, entity );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += delta_time; }
      }
   }
}    // namespace myge
