﻿#include <app/components/facing_tag.hpp>
#include <app/components/shooter.hpp>
#include <app/components/target.hpp>
#include <app/systems/facing_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{
   FacingSystem::FacingSystem( i32 priority_, entt::registry& registry_ ) : SystemInterface { priority_, registry_ } {}
   FacingSystem::~FacingSystem() {}
   void FacingSystem::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      auto  view { getLogicUpdateable<sdl_engine::Transform, sdl_engine::Velocity, FacingTag>( reg ) };
      for ( auto [ entity, trfm, velo ] : view.each() )
      {
         if ( !reg.valid( entity ) ) { continue; }

         // Stop時のみターゲット方向、移動中は進行方向を向く
         sdl_engine::Vector2_f32 dir { 0.0f, 0.0f };
         const bool              is_shooting_and_stop = reg.all_of<ShootingEnemyTag>( entity );
         if ( is_shooting_and_stop && reg.all_of<Target>( entity ) )
         {
            const auto& target = reg.get<Target>( entity );
            if ( target.is_lookat && reg.valid( target.target_entity )
                 && reg.all_of<sdl_engine::Transform>( target.target_entity ) )
            {
               auto target_trfm = reg.get<sdl_engine::Transform>( target.target_entity );
               dir              = target_trfm.position - trfm.position;
            }
         }
         // Shooting中でターゲットを向かない、または通常時はVelocityに従う
         if ( dir.lengthSq() <= 0.000001f ) { dir = velo.vector; }
         if ( dir.lengthSq() > 0.000001f ) { trfm.angle = atan2( dir.y, dir.x ) * ( 180.0f / 3.141592f ) + 90; }
      }
   }
}    // namespace myge