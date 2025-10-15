#pragma once
#include <engine/forward.hpp>
#include <engine/utils.hpp>
namespace myge
{
   struct PlayerTag
   {
   };
   struct PlayerBulletTag{};
   struct EnemyTag
   {
   };
   struct BackgroundTag
   {
   };
   void emplaceEntityTypeTag( entt::registry& registry_, entt::entity entity_, std::string_view type_ );
}    // namespace myge