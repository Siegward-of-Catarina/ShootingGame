#pragma once
#include <engine/core/forward_declarations.hpp>

namespace sdl_engine
{
   // 効果音の再生要求（未使用：コンポーネント駆動が主）
   struct PlaySEEvent
   {
      entt::entity entity;
   };
   struct StopSEEvent
   {
      i64 fadeout_time_ms;
   };
   struct StopAllSoundEvent
   {
      i64 fadeout_time_ms;
   };
   // BGMの再生要求（未使用：コンポーネント駆動が主）
   struct PlayBGMEvent
   {
      entt::entity entity;
   };

   // BGM停止要求（fadeout_time_ms ミリ秒でフェードアウト）
   struct StopBGMEvent
   {
      i64 fadeout_time_ms;
   };
}    // namespace sdl_engine