#include <SDL3/SDL_timer.h>
#include <engine/core/game_timer.hpp>
namespace sdl_engine
{
   GameTimer::GameTimer()
     : _last_time { SDL_GetPerformanceCounter() }, _delta_time { 0 }, _elapsed_time { 0 }, _frame_count { 0 },_fps{0}
   {
   }
   GameTimer::~GameTimer() {}
   void GameTimer::update()
   {
      // デルタタイムを計算
      u64 tick_time = SDL_GetPerformanceCounter();
      _delta_time   = static_cast<f64>( tick_time - _last_time ) / SDL_GetPerformanceFrequency();
      _last_time    = tick_time;

      // 経過時間を加算
      _elapsed_time += _delta_time;

      // フレームカウントアップ
      _frame_count++;

      // fps計算
      if ( _elapsed_time >= 1.0 )
      {
         _fps          = _frame_count / _elapsed_time;
         _frame_count  = 0;
         _elapsed_time = 0.0;
      }
   }
}    // namespace sdl_engine
