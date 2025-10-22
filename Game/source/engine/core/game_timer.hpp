#pragma once

#include <engine/core/forward_declarations.hpp>

namespace sdl_engine
{

   class GameTimer
   {
   public:
      GameTimer();
      ~GameTimer();
      void      update();
      void      setGameSpeed( const f32 speed_ ) { _game_speed = speed_; };
      const f32 getDeltaTime() const { return _delta_time; };
      const u32 getFPS() { return _fps; };

   private:
      f32 _game_speed;
      u64 _last_time;
      f32 _delta_time;
      f64 _elapsed_time;
      u64 _frame_count;
      u32 _fps;
   };
}    // namespace sdl_engine
