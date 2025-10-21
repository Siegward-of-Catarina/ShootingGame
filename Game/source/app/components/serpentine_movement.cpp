#include <app/components/serpentine_movement.hpp>

namespace myge
{

   SerpentineMovement createSerpentineMovement( const json& data_ )
   {
      SerpentineMovement move_comp { .center_x { 0.0f },
									 .amplitude { sdl_engine::getOptionalData<f32>(data_, "amplitude", 200.0f) },
									 .frequency {  sdl_engine::getOptionalData<f32>(data_, "frequency", 20.0f) },
									 .move_speed { sdl_engine::getOptionalData<f32>(data_, "speed", 200.0f) },
									 .move_threshold { sdl_engine::getOptionalData<f32>(data_, "threshold", 0.9f) },
                                     .time { 0.0f } };
      return move_comp;
   }
}    // namespace myge
