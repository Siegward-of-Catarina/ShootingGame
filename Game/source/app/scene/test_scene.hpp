#pragma once
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
      void proc( f32 delta_time ) override;

   private:
      void loadAssets();
      void loadSceneData();
      void createWaves();
      void createBackGrounds();

   private:
      entt::registry                     _registry;
      json                               _scene_data;
      std::vector<std::unique_ptr<Wave>> _waves;
   };
}    // namespace myge