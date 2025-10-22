#include <SDL3/SDL_timer.h>
#include <engine/core/game_timer.hpp>
namespace
{
   constexpr f32 MAX_DELTA_TIME { 0.033f };    // 30fps
}
namespace sdl_engine
{
   GameTimer::GameTimer()
     : _game_speed { 1.0f }
     , _last_time { SDL_GetPerformanceCounter() }
     , _delta_time { 0 }
     , _elapsed_time { 0 }
     , _frame_count { 0 }
     , _fps { 0 }
   {
   }
   GameTimer::~GameTimer() {}
   void GameTimer::update()
   {
      // デルタタイムを計算
      u64 tick_time = SDL_GetPerformanceCounter();
      _delta_time   = static_cast<f32>( tick_time - _last_time ) / SDL_GetPerformanceFrequency() * _game_speed;

      // ウィンドウドラッグ時などの処理スキップ時に、
      // デルタタイムが蓄積するのを防ぐ
      if ( _delta_time > MAX_DELTA_TIME ) { _delta_time = MAX_DELTA_TIME; }

      _last_time = tick_time;

      // 経過時間を加算
      _elapsed_time += _delta_time;

      // フレームカウントアップ
      _frame_count++;

      // fps計算
      if ( _elapsed_time >= 1.0 )
      {
         _fps          = static_cast<u32>( _frame_count / _elapsed_time );
         _frame_count  = 0;
         _elapsed_time = 0.0;
      }
   }
}    // namespace sdl_engine
