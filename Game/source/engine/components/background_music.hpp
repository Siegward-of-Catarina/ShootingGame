#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/sound/resource/sound_resource.hpp>

namespace sdl_engine
{
   // BGM再生用の一時トリガーコンポーネント
   // SoundSystem が検出して再生後にエンティティを破棄する
   struct BackgroundMusic
   {
      entt::resource<SoundResource> sound;              // 再生するサウンドリソース
      i64                           loop_count {};      // -1:無限, 0:ループ無し, 1:1ループ(計2回)
      i64                           fade_time_ms {};    // フェード時間（ms）
      f32                           volume {};          // 0.0f ～ 1.0f
   };
}    // namespace sdl_engine
