#pragma once
#include <engine/forward.hpp>
#include <engine/utils/json_utilities.hpp>
namespace myge
{
   struct PlayerInput
   {
      std::string up_sdl_key_name;
      std::string down_sdl_key_name;
      std::string left_sdl_key_name;
      std::string right_sdl_key_name;
   };

   PlayerInput createPlayerInput(const json& data_);
}    // namespace myge