#include <app/components/entity_type_tag.hpp>
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
      auto  view { getLogicUpdateable<sdl_engine::Transform, sdl_engine::Velocity, EnemyTag>( reg ) };
      for ( auto [ entity, trfm, velo ] : view.each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         trfm.angle = atan2( velo.dy, velo.dx ) * ( 180.0f / 3.141592f ) + 90;
      }
   }
}    // namespace myge