#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   struct Transform
   {
      f32 x;
      f32 y;
      f32 angle;
      f32 scale;
   };
   Transform createTransform(const json& data_);
   Transform createTransform();
}    // namespace sdl_engine