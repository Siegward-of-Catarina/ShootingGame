// my header
#include <app/systems/game_over_system.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/event/append_overray_fade_event.hpp>
#include <app/event/game_over_event.hpp>
namespace myge
{
   GameOverSystem::GameOverSystem( i32                        priority_,
                                   entt::registry&            registry_,
                                   sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _hit_entity_pairs {}
   {
      _event_listener.connect<&GameOverSystem::onGameOver, GameOverEvent>( this );
   }
   GameOverSystem::~GameOverSystem() {}
   void GameOverSystem::update( const sdl_engine::FrameData& frame_ )
   {
      if ( _game_over_timer > 0.0f )
      {
         _game_over_timer -= frame_.delta_time;
         if ( _game_over_timer <= 0.0f )
         {
            // fade
            _event_listener.trigger<AppendOverrayFadeEvent>( {} );
         }
      }
   }
   void GameOverSystem::onGameOver( GameOverEvent& e ) { _game_over_timer = 1.0f; }
}    // namespace myge