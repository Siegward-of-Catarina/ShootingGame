#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   class MovementSystem : public SystemInterface
   {
   public:
      MovementSystem( i32 priority_, entt::registry& registry_ );
      virtual ~MovementSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( EngineContext& context_ ) override;
   };
}    // namespace sdl_engine
