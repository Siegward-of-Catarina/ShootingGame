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
     : _dispatchar { dispatcher_ }
     , _fade {}
     , _current_scene { nullptr }
     , _next_scene { nullptr }
     , _enable_update { true }
   {
      _dispatchar.sink<FadeOutEndEvent>().connect<&SceneManager::onFadeOutEnd>( this );
      _dispatchar.sink<FadeInEndEvent>().connect<&SceneManager::onFadeInEnd>( this );
   }
   SceneManager::~SceneManager() {}
   void SceneManager::update( EngineContext& context_ )
   {

      auto& registry { context_.getRegistry() };
      if ( _next_scene && _enable_update )
      {
         _dispatchar.trigger<FadeOutStartEvent>( { _fade.value() } );
         _enable_update = false;
      }

      if ( _enable_update ) { _current_scene->update( context_.getGameTimer().getDeltaTime() ); }
   }
   void SceneManager::initStartCurrentScene( std::unique_ptr<Scene> current_ )
   {
      _current_scene = std::move( current_ );
      _current_scene->initialize( _dispatchar );
      _current_scene->start();
   }
   void SceneManager::onFadeOutEnd( FadeOutEndEvent& e )
   {
      if ( _fade == e.owner )
      {
         _current_scene = std::move( _next_scene );
         _current_scene->initialize( _dispatchar );
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