#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
namespace myge
{
   struct MenuButtonEvent;
   struct KeyDownEvent;
   class GameClearScene final : public sdl_engine::Scene
   {

   public:
      GameClearScene( const sdl_engine::SceneDependencies& dependencies_ );
      virtual ~GameClearScene() override;
      virtual void start() override;
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      // Scene を介して継承されました
      virtual void addSystems() override;
      virtual void createEntities() override;
      virtual void setupEventHandlers() override;

   private:
      void onContinueMenuAction( const MenuButtonEvent& e );
      void onAppendInputSE( const KeyDownEvent& e );
   };
}    // namespace myge
