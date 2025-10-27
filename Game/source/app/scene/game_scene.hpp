#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
#include <engine/utils.hpp>
namespace myge
{
   struct ShootEvent;
   struct GameOverEvent;
   struct AppendDeadEffectEvent;
   struct AppendOverrayFadeEvent;
   class Wave;
   class GameScene final : public sdl_engine::Scene
   {
   public:
      GameScene( const sdl_engine::SceneDependencies& dependencies_ );
      virtual ~GameScene() override;
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
      void onGameOver( GameOverEvent& e );
      void onDeadEffectAppend( AppendDeadEffectEvent& e );
      void onOverrayFadeAppend( AppendOverrayFadeEvent& e );

   private:
      std::vector<std::unique_ptr<Wave>> _waves;
      f64                                _scene_elapsed_time;
   };
}    // namespace myge