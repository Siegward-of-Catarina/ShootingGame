#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class FacingSystem final : public sdl_engine::SystemInterface
   {
   public:
      FacingSystem( i32 priority_, entt::registry& registry_ );
      ~FacingSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;
   };
}    // namespace myge
