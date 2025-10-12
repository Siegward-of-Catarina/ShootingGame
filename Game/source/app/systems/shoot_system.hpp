#pragma once
#include <engine/systems/system_interface.hpp>
namespace myge
{
   class ShootSystem final : public sdl_engine::SystemInterface
   {
   public:
      ShootSystem( i32 priority_ );
      virtual ~ShootSystem() override;

   private:
      // SystemInterface を介して継承されました
      virtual void update( sdl_engine::GameContext& context_ ) override;
   };
}    // namespace myge