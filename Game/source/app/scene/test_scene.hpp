#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
#include <engine/utils.hpp>
namespace myge
{
   struct ShootEvent;
   struct AppedDeadEffectEvent;
   class Wave;
   class TestScene final : public sdl_engine::Scene
   {
   public:
      TestScene( const sdl_engine::SceneDependencies& dependencies_ );
      virtual ~TestScene() override;
      virtual void start() override;
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      virtual void addSystems() override;
      virtual void createEntities() override;
      virtual void postSystemAddition() override;
      virtual void setupEventHandlers() override;

   private:
      void createWaves();
      void onShoot( ShootEvent& e );
      void onDeadEffectAppend( AppedDeadEffectEvent& e );

   private:
      std::vector<std::unique_ptr<Wave>> _waves;
      f64                                _scene_elapsed_time;
   };
}    // namespace myge