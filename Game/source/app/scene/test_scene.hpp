#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
#include <engine/utils.hpp>
namespace myge
{
   struct ShootEvent;
   class Wave;
   class TestScene final : public sdl_engine::Scene
   {
   public:
      TestScene( const sdl_engine::SceneDependencies& dependencies_ );
      ~TestScene();
      // Scene を介して継承されました
      virtual void initialize() override;
      virtual void start() override;
      virtual void update( f32 deita_time_ ) override;

      virtual void addSystems() override;

   private:
      void createWaves();
      void onShoot( ShootEvent& e );

   private:
      std::vector<std::unique_ptr<Wave>> _waves;
      f64                                _scene_elapsed_time;
      entt::dispatcher                   _disp;
   };
}    // namespace myge