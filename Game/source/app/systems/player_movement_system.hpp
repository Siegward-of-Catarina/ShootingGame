#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class PlayerMovementSystem final : public sdl_engine::SystemInterface
   {
   public:
      PlayerMovementSystem();
      virtual ~PlayerMovementSystem() override;

      // SystemInterface ����Čp������܂���
      virtual void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_time_ ) override;
      virtual int  priority() const override;
   };
}    // namespace myge
