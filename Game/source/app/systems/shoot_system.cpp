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

   void ShootSystem::update(const sdl_engine::FrameData& frame_)
   {
      for ( auto [ entity, shooter, input ] : getLogicUpdateable<Shooter, PlayerInput>( registry() ).each() )
      {
         if ( shooter.wait > shooter.cooldown && input.isShoot )
         {
            shooter.wait = 0.0f;
            _disp.trigger( ShootEvent { entity } );
         }
         else if ( shooter.wait <= shooter.cooldown ) { shooter.wait += frame_.delta_time; }
      }
   }
}    // namespace myge
