#pragma once
#include <engine/forward.hpp>
#include <engine/scene/scene.hpp>
namespace myge
{
   class TitleScene final : public sdl_engine::Scene
   {
   public:
      TitleScene( sdl_engine::GameContext& ctx_ );
      virtual ~TitleScene() override;
      // Scene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
      void initialize() override;
      void start() override;
      void update() override;
   };
}    // namespace myge
