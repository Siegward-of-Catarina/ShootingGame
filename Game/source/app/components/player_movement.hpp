#pragma once
#include <engine/forward.hpp>
namespace myge
{
   enum class PlayerMovementType
   {
      AutoPilot,
      InputMovement
   };
   struct PlayerMovement
   {
      f32 speed {};
   };
   struct PlayerAutoPilotMove
   {
      PlayerMovementType type {};
      f32                auto_pilot_time {};
   };
}    // namespace myge