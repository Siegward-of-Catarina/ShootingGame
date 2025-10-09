#include <app/components/player_input.hpp>

namespace myge {
	PlayerInput createPlayerInput(const json& data_)
	{
		PlayerInput input_comp{
				   .up_sdl_key_name { sdl_engine::getJsonData<std::string>(data_, "up_sdl_key_name").value() },
				   .down_sdl_key_name { sdl_engine::getJsonData<std::string>(data_, "down_sdl_key_name").value() },
				   .left_sdl_key_name { sdl_engine::getJsonData<std::string>(data_, "left_sdl_key_name").value() },
				   .right_sdl_key_name { sdl_engine::getJsonData<std::string>(data_, "right_sdl_key_name").value() }
		};

		return input_comp;
	}
}