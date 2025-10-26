#pragma once
#include <engine/core/forward_declarations.hpp>
#include <typeindex>

namespace sdl_engine
{
   class SpriteAnimationSystem;
   class SpriteRenderSystem;
   using SystemInterfacePtr = std::unique_ptr<SystemInterface>;

   // System のライフサイクルと実行順を管理
   // - 追加/削除/取得
   // - priority に基づく実行順の再構築
   class SystemManager
   {
   public:
      SystemManager( entt::registry& registry_,
                     Renderer&       renderer_,
                     SoundMixer&     soundmixer_,
                     EventListener&  event_listener_ );
      ~SystemManager();

      // すべての System を priority 順に更新
      void updateSystems( const sdl_engine::FrameData& frame_ );

      // すべての System を解放
      void clearSystems();

      // System を追加（T は SystemInterface 派生）
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& addSystem( std::unique_ptr<T> system_ )
      {
         auto& idx { typeid( T ) };
         if ( _to_remove.size() > 0 )
         {
            // 予約削除があれば先に反映
            auto it = std::find( _to_remove.begin(), _to_remove.end(), idx );
            if ( it != _to_remove.end() )
            {
               *it = std::move( _to_remove.back() );
               _to_remove.pop_back();
            }
         }
         _systems[ idx ]     = std::move( system_ );
         _needs_rebuild_view = true;
         return *static_cast<T*>( _systems[ idx ].get() );
      };

      // System を削除予約（実行中に破棄せず次回再構築時に反映）
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      void removeSystem()
      {
         auto& id { typeid( T ) };
         _to_remove.emplace_back( id );
      }

      // 追加済みの System を取得
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& getSystem()
      {
         auto& idx { typeid( T ) };
         return *static_cast<T*>( _systems[ idx ].get() );
      };

   private:
      // priority に基づき実行順の配列を再構築
      void rebuildSystemView();

   private:
      std::map<std::type_index, SystemInterfacePtr> _systems;
      std::vector<SystemInterface*>                 _sorted_view;
      std::vector<std::type_index>                  _to_remove;
      SystemInterfacePtr                            _sprite_animation_system;
      SystemInterfacePtr                            _render_system;
      SystemInterfacePtr                            _sound_system;
      bool                                          _needs_rebuild_view;
   };
}    // namespace sdl_engine