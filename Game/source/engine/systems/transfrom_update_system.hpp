#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class TransformUpdateSystem : public SystemInterface
   {
   public:
      TransformUpdateSystem( i32 priority_ );
      ~TransformUpdateSystem() override;
      // SystemInterface ����Čp������܂���
      virtual void update( GameContext& context_ ) override;
   };
}    // namespace sdl_engine
