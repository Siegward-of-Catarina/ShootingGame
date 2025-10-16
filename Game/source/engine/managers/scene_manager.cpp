#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/events/fade_events.hpp>
#include <engine/graphics.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/scene/scene.hpp>
#include <engine/systems/fade_system.hpp>

namespace sdl_engine
{
   SceneManager::SceneManager( entt::dispatcher& dispatcher_ )
     : _dispatcher { dispatcher_ }
     , _fade {}
     , _current_scene { nullptr }
     , _next_scene { nullptr }
     , _enable_update { true }
   {
      _dispatcher.sink<FadeOutEndEvent>().connect<&SceneManager::onFadeOutEnd>( this );
      _dispatcher.sink<FadeInEndEvent>().connect<&SceneManager::onFadeInEnd>( this );
   }
   SceneManager::~SceneManager()
   {
      _dispatcher.sink<FadeOutEndEvent>().disconnect<&SceneManager::onFadeOutEnd>( this );
      _dispatcher.sink<FadeInEndEvent>().disconnect<&SceneManager::onFadeInEnd>( this );
   }
   void SceneManager::update( const FrameData& frame_ )
   {
      if ( _next_scene && _enable_update )
      {
         _dispatcher.trigger<FadeOutStartEvent>( { _fade.value() } );
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
   void SceneManager::onFadeOutEnd( FadeOutEndEvent& e )
   {
      if ( _fade == e.owner )
      {
         _current_scene = std::move( _next_scene );
         _current_scene->initialize();
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
}    // namespace sdl_engine