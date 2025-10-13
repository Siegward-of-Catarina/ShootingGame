#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
namespace myge
{
   class TitleScene final : public sdl_engine::Scene
   {
   public:
      TitleScene( const sdl_engine::SceneDependencies& dependencies_ );
      virtual ~TitleScene() override;
      // Scene を介して継承されました
      void initialize() override;
      void start() override;
      void update( f32 deita_time_ ) override;

   private:
      // Scene を介して継承されました
      virtual void addSystems() override;

   private:
      f64 _scene_elapsed_time;
   };
}    // namespace myge
