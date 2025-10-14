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
      SystemManager( entt::registry& registry_, Renderer& renderer_ );
      ~SystemManager();
      void updateSystems( const sdl_engine::FrameData& frame_ );
      // T型がSystemInterfaceを基底に持つもののみ通す
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& addSystem( std::unique_ptr<T> system_ )
      {
         auto& idx { typeid( T ) };
         _systems[ idx ]     = std::move( system_ );
         _needs_rebuild_view = true;
         return *static_cast<T*>( _systems[ idx ].get() );
      };
      // T型がSystemInterfaceを基底に持つもののみ通す
      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      void removeSystem()
      {
         auto& id { typeid( T ) };
         _systems.erase( id );
         _needs_rebuild_view = true;
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
      std::unique_ptr<SpriteAnimationSystem>        _sprite_animation_system;
      std::unique_ptr<SpriteRenderSystem>           _render_system;
      bool                                          _needs_rebuild_view;
   };
}    // namespace sdl_engine
