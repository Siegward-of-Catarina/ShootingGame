#pragma once
#include <engine/core/forward_declarations.hpp>
#include <typeindex>
namespace sdl_engine
{
   using SystemInterfacePtr = std::unique_ptr<SystemInterface>;
   class SystemManager
   {
   public:
      SystemManager();
      ~SystemManager();
      void addSystem( const std::type_index& id_, SystemInterfacePtr system_ );
      void updateSystems( GameContext& context_ );
      void removeSystem( const std::type_index& id_ );

   private:
      void rebuildSystemView();

   private:
      std::map<std::type_index, SystemInterfacePtr> _systems;
      std::vector<SystemInterface*>                 _sorted_view;
      std::vector<SystemInterfacePtr>               _basic_systems;
      bool                                          _needs_rebuild_view;
   };
}    // namespace sdl_engine
