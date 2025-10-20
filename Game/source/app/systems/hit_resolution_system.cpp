// my header
#include <app/systems/hit_resolution_system.hpp>
// core
#include <engine/core.hpp>
// component
#include <app/components/damage.hpp>
#include <app/components/status.hpp>
// event
#include <app/event/append_dead_effect_event.hpp>
#include <app/event/hit_event.hpp>
namespace myge
{
   HitResolutionSystem::HitResolutionSystem( i32                        priority_,
                                             entt::registry&            registry_,
                                             sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
      _event_listener.connect<&HitResolutionSystem::onHit, HitEvent>( this );
   }
   HitResolutionSystem::~HitResolutionSystem() {}
   void HitResolutionSystem::update( const sdl_engine::FrameData& frame_ ) {}
   void HitResolutionSystem::onHit( HitEvent& e )
   {
      auto&                     reg = registry();
      std::vector<entt::entity> dead_entities;
      std::vector<entt::entity> damage_entities;
      for ( auto [ entity_a, entity_b ] : e.hit_entity_pairs )
      {
         if ( reg.valid( entity_a ) && reg.valid( entity_b ) )
         {
            auto& status_a = reg.get<Status>( entity_a );
            auto& status_b = reg.get<Status>( entity_b );
            status_a.hp -= status_b.atk;
            status_b.hp -= status_a.atk;

            if ( status_a.hp <= 0 ) { dead_entities.emplace_back( entity_a ); }
            else { damage_entities.emplace_back( entity_a ); }
            if ( status_b.hp <= 0 ) { dead_entities.emplace_back( entity_b ); }
         }
      }
      for ( auto entity : damage_entities )
      {
         if ( !reg.valid( entity ) || reg.all_of<DamageEffect>( entity ) ) { continue; }
         reg.emplace<DamageEffect>( entity, DamageEffect { 1.0f, 1.0f / 10, 0.0f } );
      }
      _event_listener.trigger<AppedDeadEffectEvent>( { dead_entities } );
   }
}    // namespace myge