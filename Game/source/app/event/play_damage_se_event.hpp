#pragma once
#include <engine/forward.hpp>
namespace myge
{
   // ダメージSE再生要求イベント
   struct PlayDamageSEEvent
   {
      std::vector<entt::entity> damaged_entities {};
   };
}    // namespace myge