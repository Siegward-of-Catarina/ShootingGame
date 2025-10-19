#pragma once
#include <engine/forward.hpp>
#include <engine/rendering/resource/color.hpp>
namespace myge
{
   struct Highlightable
   {
      bool                  active {};
      sdl_engine::ColorRGBA active_color {};
      sdl_engine::ColorRGBA inactive_color {};
   };
}    // namespace myge