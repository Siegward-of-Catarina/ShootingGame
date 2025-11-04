#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct SpriteBrink
   {
      f32 speed {};
      f32 min_alpha {};
      i32 num_cycles { -1 };    // 点滅回数 -1で無限
      // Add ,Sub どちらから始まっても対応できるようにするためのフラグ　カウント２で１サイクル
      u32 is_cycle_end { 0 };
      enum class State
      {
         AddAlpha,
         SubAlpha
      } state { State::SubAlpha };
   };
}    // namespace myge