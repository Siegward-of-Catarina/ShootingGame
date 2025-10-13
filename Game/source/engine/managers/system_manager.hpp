#pragma once
#include <engine/core/forward_declarations.hpp>
#include <typeindex>
namespace sdl_engine
{
   using SystemInterfacePtr = std::unique_ptr<SystemInterface>;
   class SystemManager
   {
   public:
      SystemManager( entt::registry& registry_, Renderer& renderer_ );
      ~SystemManager();
      void updateSystems( EngineContext& context_ );

      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      T& addSystem( std::unique_ptr<T> system_ )
      {
         auto& idx { typeid( T ) };
         _systems[ idx ]     = std::move( system_ );
         _needs_rebuild_view = true;
         return *static_cast<T*>( _systems[ idx ].get() );
      };

      template<typename T, typename = std::enable_if<std::is_base_of<SystemInterface, T>::value>>
      void removeSystem()
      {
         auto& id { typeid( T ) };
         _systems.erase( id );
         _needs_rebuild_view = true;
      }

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
      std::vector<SystemInterfacePtr>               _basic_systems;
      bool                                          _needs_rebuild_view;
   };
}    // namespace sdl_engine
