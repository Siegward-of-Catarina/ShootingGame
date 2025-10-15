#pragma once
#include <entt/entt.hpp>
namespace sdl_engine
{
   struct FadeOutStartEvent
   {
      entt::entity owner;
   };
   struct FadeOutEndEvent
   {
      entt::entity owner;
   };
   struct FadeInEndEvent
   {
      entt::entity owner;
   };
}    // namespace sdl_engine