#pragma once
#include <engine/core/forward_declarations.hpp>
#include <typeindex>
namespace sdl_engine
{
   class SpriteAnimationSystem;
   class SpriteRenderSystem;
   using SystemInterfacePtr = std::unique_ptr<SystemInterface>;
   class SystemManager
   {
   public:
      SystemManager( entt::registry& registry_, Renderer& renderer_, EventListener& event_listener_ );
      ~SystemManager();
      void updateSystems( const sdl_engine::FrameData& frame_ );
      void clearSystems();
      // T型がSystemInterfaceを基底に持つもののみ通す
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& addSystem( std::unique_ptr<T> system_ )
      {
         auto& idx { typeid( T ) };
         if ( _to_remove.size() > 0 )
         {
            // 削除予約されているものがあれば削除
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
      // T型がSystemInterfaceを基底に持つもののみ通す
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      void removeSystem()
      {
         auto& id { typeid( T ) };
         _to_remove.emplace_back( id );
      }
      // T型がSystemInterfaceを基底に持つもののみ通す
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& getSystem()
      {
         auto& idx { typeid( T ) };
         return *static_cast<T*>( _systems[ idx ].get() );
      };

   private:
      void rebuildSystemView();

   private:
      std::map<std::type_index, SystemInterfacePtr> _systems;
      std::vector<SystemInterface*>                 _sorted_view;
      std::vector<std::type_index>                  _to_remove;
      SystemInterfacePtr                            _sprite_animation_system;
      SystemInterfacePtr                            _render_system;
      bool                                          _needs_rebuild_view;
   };
}    // namespace sdl_engine
