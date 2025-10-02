#include <engine/basic_system.hpp>
#include <engine/managers/system_manager.hpp>
namespace sdl_engine
{

   SystemManager::SystemManager() : _systems {}, _sorted_view {}, _basic_systems { 3 }, _needs_rebuild_view { true }
   {
      // �x�[�V�b�N�ȃV�X�e����ǉ�
      // �����͕ʘg�ŊǗ�����ΓI�ɂ��̏��ԂŎ��s�����
      _basic_systems[ 0 ] = std::make_unique<TransformUpdateSystem>( 98 );
      // �A�j���[�V�����͕K�v�Ȃ���ʂ����邩������Ȃ����A
      // size�O�Ń��[�v������邾���Ȃ̂ŋ��e����
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
      // �D��x���ŕ��ׂ�
      std::sort( _sorted_view.begin(),
                 _sorted_view.end(),
                 []( const auto& rref, const auto& lref ) { return rref->getPriority() < lref->getPriority(); } );
   }
   void SystemManager::updateSystems( GameContext& context_ )
   {
      // map�ɕύX���������ꍇview���č\�z
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
