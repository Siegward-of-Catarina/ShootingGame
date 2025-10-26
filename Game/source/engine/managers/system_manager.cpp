#include <engine/basic_system.hpp>
#include <engine/managers/system_manager.hpp>
namespace sdl_engine
{

   SystemManager::SystemManager( entt::registry& registry_,
                                 Renderer&       renderer_,
                                 SoundMixer&     soundmixer_,
                                 EventListener&  event_listener_ )
     : _systems {}
     , _sorted_view {}
     , _to_remove {}
     , _sprite_animation_system { std::make_unique<SpriteAnimationSystem>( 99, registry_, event_listener_ ) }
     , _render_system { std::make_unique<SpriteRenderSystem>( 100, registry_, renderer_ ) }
     , _sound_system { std::make_unique<SoundSystem>( 98, registry_, event_listener_, soundmixer_ ) }
     , _needs_rebuild_view { true }
   {
   }
   SystemManager::~SystemManager() {}

   void SystemManager::rebuildSystemView()
   {
      _sorted_view.resize( _systems.size() );
      for ( i32 i = 0; const auto& system : _systems )
      {
         if ( system.second ) { _sorted_view[ i++ ] = system.second.get(); }
      }
      // 優先度順で並べる
      std::sort( _sorted_view.begin(),
                 _sorted_view.end(),
                 []( const auto& rref, const auto& lref ) { return rref->priority() < lref->priority(); } );
      _to_remove.clear();
      _needs_rebuild_view = false;
   }
   void SystemManager::updateSystems( const sdl_engine::FrameData& frame_ )
   {
      // 削除予約されているシステムを削除
      for ( auto& id : _to_remove )
      {
         _systems.erase( id );
         _needs_rebuild_view = true;
      }
      // mapに変更があった場合viewを再構築
      if ( _needs_rebuild_view ) { rebuildSystemView(); }

      for ( auto& system : _sorted_view ) { system->update( frame_ ); }

      // 固定system
      _sound_system->update( frame_ );
      _sprite_animation_system->update( frame_ );
      _render_system->update( frame_ );
   }
   void SystemManager::clearSystems()
   {
      _systems.clear();
      _to_remove.clear();
   }
}    // namespace sdl_engine
