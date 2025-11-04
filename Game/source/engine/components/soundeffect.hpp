#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/sound/resource/sound_resource.hpp>

namespace sdl_engine
{
   // 効果音再生用の一時トリガーコンポーネント
   // SoundSystem が検出して再生後にエンティティを破棄する
   struct SoundEffect
   {
      entt::resource<SoundResource> sound;            // 再生するサウンドリソース
      i64                           loop_count;       // -1:無限, 0:ループ無し, 1:1ループ(計2回)
      f32                           volume;           // 0.0f ～ 1.0f
      f32                           delay { 0 };      // 再生までの遅延秒
      f32                           elapsed { 0 };    // 経過秒
   };
}    // namespace sdl_engine