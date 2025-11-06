#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class TransformLinkSystem final : public sdl_engine::SystemInterface
   {
   public:
      TransformLinkSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~TransformLinkSystem();
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      sdl_engine::EventListener& _event_listener;
      std::vector<entt::entity>  _dead_link_entities;
   };
}    // namespace myge
