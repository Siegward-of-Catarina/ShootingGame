#pragma once
#include <engine/forward.hpp>

namespace myge
{
   struct TitleMenu
   {
      std::vector<entt::entity> menu_ui;
      u32                       selected {};
   };
}    // namespace myge
