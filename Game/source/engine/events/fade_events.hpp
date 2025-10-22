#pragma once
#include <entt/entt.hpp>
namespace sdl_engine
{
   struct FadeOutStartEvent
   {
      entt::entity owner;
      f32          end_alpha_override { -1.0f };
      f32          target_out_alpha_override { -1.0f };
      f32          speed_override { -1.0f };
      f32          black_out_duration_override { -1.0f };
      f32          start_alpha_override { -1.0f };
   };
   struct FadeOutEndEvent
   {
      entt::entity owner;
   };
   struct FadeInEndEvent
   {
      entt::entity owner;
   };
   struct FadeSetAlphaEvent
   {
      entt::entity owner;
      f32          alpha;
   };
}    // namespace sdl_engine