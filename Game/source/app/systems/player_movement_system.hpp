#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class PlayerMovementSystem final : public sdl_engine::SystemInterface
   {
   public:
      PlayerMovementSystem( i32 priority_ );
      virtual ~PlayerMovementSystem() override;

      // SystemInterface ����Čp������܂���
      virtual void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge
