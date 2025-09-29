#pragma once
#include <engine/core/forward_declarations.hpp>
namespace myge {
	enum class PlayerMovementType {
		AutoPilot,
		InputMovement
	};
	struct PlayerMovement {
		f32 speed;
	};
	struct PlayerAutoPilotMove {
		PlayerMovementType type;
		f32 auto_pilot_time;
	};
	struct PlayerInputMove {
		u32 up_code;
		u32 down_code;
		u32 left_code;
		u32 right_code;
	};
}