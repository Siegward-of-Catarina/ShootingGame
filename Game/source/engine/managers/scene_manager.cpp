#include <pch.hpp>
// my header
#include <engine/managers/scene_manager.hpp>
// core
#include <engine/core.hpp>
// graphics
#include <engine/graphics.hpp>
// scene
#include <engine/scene/scene.hpp>
// component
#include <engine/basic_component.hpp>
// system
#include <engine/systems/fade_system.hpp>
// event
#include <engine/events/fade_events.hpp>
#include <engine/events/quit_event.hpp>

namespace sdl_engine
{
   SceneManager::SceneManager( entt::dispatcher& dispatcher_, GameTimer& game_timer_ )
     : _event_listener { std::make_unique<EventListener>( dispatcher_ ) }
     , _fade {}
     , _game_timer { game_timer_ }
     , _current_scene { nullptr }
     , _next_scene { nullptr }
     , _enable_update { true }
   {
      _event_listener->connect<&SceneManager::onFadeOutEnd, FadeOutEndEvent>( this );
      _event_listener->connect<&SceneManager::onFadeInEnd, FadeInEndEvent>( this );
   }
   SceneManager::~SceneManager() {}
   void SceneManager::update( const FrameData& frame_ )
   {
      if ( _next_scene && _enable_update )
      {
         _event_listener->trigger<FadeOutStartEvent>( { _fade.value() } );
         _enable_update = false;
      }

      if ( _enable_update ) { _current_scene->update( frame_ ); }
   }
   void SceneManager::initStartCurrentScene( std::unique_ptr<Scene> current_ )
   {
      _current_scene = std::move( current_ );
      _current_scene->initialize();
      _current_scene->start();
   }
   void SceneManager::quitGame()
   {
      // ここではフェードを呼ぶだけ
      _event_listener->trigger<FadeOutStartEvent>( { _fade.value() } );
   }
   void SceneManager::onFadeOutEnd( FadeOutEndEvent& e )
   {
      if ( _fade == e.owner )
      {
         if ( _next_scene )
         {
            _current_scene = std::move( _next_scene );
            _current_scene->initialize();
         }
         else
         {    // quitGame時はnext_sceneはセットされないためここで判定する
            _event_listener->trigger<QuitEvent>( {} );
         }
      }
   }
   void SceneManager::onFadeInEnd( FadeInEndEvent& e )
   {
      if ( _fade == e.owner )
      {
         _current_scene->start();
         _enable_update = true;
      }
   }
   void SceneManager::setGameSpeed( const f32 speed_ ) { _game_timer.setGameSpeed( speed_ ); }
}    // namespace sdl_engine