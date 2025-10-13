#include <engine/basic_system.hpp>
#include <engine/managers/system_manager.hpp>
namespace sdl_engine
{

   SystemManager::SystemManager( entt::registry& registry_, Renderer& renderer_ )
     : _systems {}, _sorted_view {}, _basic_systems { 2 }, _needs_rebuild_view { true }
   {
      // size０でループを避けるだけなので許容する
      _basic_systems[ 0 ] = std::make_unique<SpriteAnimationSystem>( 99, registry_ );
      _basic_systems[ 1 ] = std::make_unique<SpriteRenderSystem>( 100, registry_, renderer_ );
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
                 []( const auto& rref, const auto& lref ) { return rref->getPriority() < lref->getPriority(); } );
   }
   void SystemManager::updateSystems( EngineContext& context_ )
   {
      // mapに変更があった場合viewを再構築
      if ( _needs_rebuild_view ) { rebuildSystemView(); }

      for ( auto& system : _systems )
      {
         //
         system.second->update( context_ );
      }
      for ( auto& basic_sys : _basic_systems ) { basic_sys->update( context_ ); }
   }
}    // namespace sdl_engine
