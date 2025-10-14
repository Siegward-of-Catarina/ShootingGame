#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
    struct FadeOutStartEvent;
   class FadeSystem : public SystemInterface
   {
   public:
      FadeSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& dispatchar_ );
      virtual ~FadeSystem() override;
      // SystemInterface を介して継承されました
      virtual void update(const FrameData& frame_) override;
   private:
       void onFadeOutStart(FadeOutStartEvent& e);
   private:
       entt::dispatcher& _dispatchar;
       std::vector<entt::entity> _fades;
   };
}    // namespace sdl_engine
