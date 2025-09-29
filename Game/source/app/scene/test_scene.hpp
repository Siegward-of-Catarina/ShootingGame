#pragma once
#include <app/systems/player_movement_system.hpp>
#include <engine/core/forward_declarations.hpp>
#include <engine/scene/scene.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class Wave;
   class TestScene : public sdl_engine::Scene
   {
   public:
      TestScene( sdl_engine::GameContext& ctx_ );
      ~TestScene();
      // Scene を介して継承されました
      void proc( f32 delta_time_ ) override;

   private:
      void loadAssets();
      void loadSceneData();
      void createWaves();

   private:
      entt::registry                                            _registry;
      json                                                      _scene_data;
      std::vector<std::unique_ptr<Wave>>                        _waves;
      std::unique_ptr<PlayerMovementSystem>                     _player_movement_system;
      std::vector<std::unique_ptr<sdl_engine::SystemInterface>> _systems;
      f64                                                       _scene_elapsed_time;
   };
}    // namespace myge