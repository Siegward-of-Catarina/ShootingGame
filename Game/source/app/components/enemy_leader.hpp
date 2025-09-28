#pragma once
#include <deque>
#include <engine/components/transform.hpp>
namespace myge
{
   struct EnemyLeader
   {
      std::deque<sdl_engine::Transform> history;
      size_t                            max_history_size {};
   };
}    // namespace myge