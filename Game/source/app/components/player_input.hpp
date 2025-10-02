#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct PlayerInput
   {
      std::string up_sdl_key_name;
      std::string down_sdl_key_name;
      std::string left_sdl_key_name;
      std::string right_sdl_key_name;
   };
}    // namespace myge