#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct Target
   {
      entt::entity target_entity { entt::null };
      bool         is_lookat { true };    // true: FacingSystemで常時ターゲット方向を向く
   };
}    // namespace myge