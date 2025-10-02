#include <engine/basic_system.hpp>
#include <engine/managers/system_manager.hpp>
namespace sdl_engine
{

   SystemManager::SystemManager() : _systems {}, _sorted_view {}, _basic_systems { 3 }, _needs_rebuild_view { true }
   {
      // ベーシックなシステムを追加
      // これらは別枠で管理し絶対的にこの順番で実行される
      _basic_systems[ 0 ] = std::make_unique<TransformUpdateSystem>( 98 );
      // アニメーションは必要ない場面もあるかもしれないが、
      // size０でループを避けるだけなので許容する
      _basic_systems[ 1 ] = std::make_unique<SpriteAnimationSystem>( 99 );
      _basic_systems[ 2 ] = std::make_unique<SpriteRenderSystem>( 100 );
   }
   SystemManager::~SystemManager() {}
   void SystemManager::addSystem( const std::type_index& id_, SystemInterfacePtr system_ )
   {
      _systems[ id_ ]     = std::move( system_ );
      _needs_rebuild_view = true;
   }
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
   void SystemManager::updateSystems( GameContext& context_ )
   {
      // mapに変更があった場合viewを再構築
      if ( _needs_rebuild_view ) { rebuildSystemView(); }

      for ( auto& system : _systems ) { system.second->update( context_ ); }
      for ( auto& basic_sys : _basic_systems ) { basic_sys->update( context_ ); }
   }
   void SystemManager::removeSystem( const std::type_index& id_ )
   {
      _systems.erase( id_ );
      _needs_rebuild_view = true;
   }
}    // namespace sdl_engine
