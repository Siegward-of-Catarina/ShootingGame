#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/math.hpp>
namespace myge
{
   struct TransformLink
   {
      entt::entity            parent {};
      sdl_engine::Vector2_f32 offset_pos;
   };
}    // namespace myge