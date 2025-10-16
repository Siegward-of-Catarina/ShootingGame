#pragma once

#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   struct SceneDependencies
   {
      entt::registry&   registry;
      entt::dispatcher& dispatcher;
      ResourceManager&  resource_manager;
      InputManager&     input_manager;
      SceneManager&     scene_manager;
      SystemManager&    system_manager;
   };
   class Scene
   {
   public:
      Scene( const SceneDependencies& dependencies_ );
      virtual ~Scene();
      virtual void initialize()                      = 0;
      virtual void start()                           = 0;
      virtual void update( const FrameData& frame_ ) = 0;

   protected:
      void         loadSceneData( std::string_view data_path_ );
      virtual void addSystems() = 0;

   protected:
      void                       setEntities( const std::vector<entt::entity>& entities_ ) { _entities = entities_; }
      std::vector<entt::entity>& entities() { return _entities; }
      json&                      sceneData() { return _scene_data; }
      entt::registry&            registry() { return _registry; };
      entt::dispatcher&          dispatcher() { return _dispatcher; }
      ResourceManager&           resourceManager() { return _resource_manager; };
      InputManager&              inputManager() { return _input_manager; }
      SceneManager&              sceneManager() { return _scene_manager; };
      SystemManager&             systemManager() { return _system_manager; };
      SceneDependencies          sceneDependencies()
      {
         return { _registry, _dispatcher, _resource_manager, _input_manager, _scene_manager, _system_manager };
      };

   private:
      entt::registry&   _registry;
      entt::dispatcher& _dispatcher;
      ResourceManager&  _resource_manager;
      InputManager&     _input_manager;
      SceneManager&     _scene_manager;
      SystemManager&    _system_manager;

   private:
      std::vector<entt::entity> _entities;
      json                      _scene_data;
   };
}    // namespace sdl_engine