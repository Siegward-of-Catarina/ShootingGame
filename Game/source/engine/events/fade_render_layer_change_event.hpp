#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   struct FadeRenderLayerChangeEvent
   {
      entt::entity fade_entity;
      bool         under_ui;
   };
}    // namespace sdl_engine