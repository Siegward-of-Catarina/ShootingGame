#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
#include <engine/utils.hpp>
namespace myge
{
   class Wave;
   class TestScene : public sdl_engine::Scene
   {
   public:
      TestScene( sdl_engine::GameContext& ctx_ );
      ~TestScene();
      // Scene を介して継承されました
      void initialize() override;
      void start() override;
      void update() override;

   private:
      void loadAssets();
      void loadSceneData();
      void createWaves();

   private:
      json                               _scene_data;
      std::vector<std::unique_ptr<Wave>> _waves;
      f64                                _scene_elapsed_time;
      std::vector<entt::entity>          _entities;
   };
}    // namespace myge