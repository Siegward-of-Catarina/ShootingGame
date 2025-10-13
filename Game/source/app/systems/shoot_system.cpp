#include <app/components/player_input.hpp>
#include <app/components/shooter.hpp>
#include <app/entity_factory.hpp>
#include <app/systems/shoot_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
//
#include <app/event/shoot_event.hpp>
namespace myge
{

   ShootSystem::ShootSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& disp_ )
     : SystemInterface { priority_, registry_ }, _disp { disp_ }
   {
   }

   ShootSystem::~ShootSystem() {}

   void ShootSystem::update( sdl_engine::EngineContext& context_ )
   {
      auto& registry { getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };
      for ( auto [ entity, shooter, input ] : getLogicUpdateable<Shooter, PlayerInput>( registry ).each() )
      {
         if ( shooter.wait > shooter.cooldown && input.isShoot )
         {
            shooter.wait = 0.0f;
            EntityFactory factory;
            // factory.createBullet( context_, entity );
            _disp.trigger( ShootEvent { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += delta_time; }
      }
   }
}    // namespace myge
