#include <app/components/serpentine_movement.hpp>

namespace myge
{

   SerpentineMovement createSerpentineMovement( const json& data_ )
   {
      SerpentineMovement move_comp { .center_x { sdl_engine::getJsonData<f32>( data_, "center_x" ).value() },
                                     .amplitude { sdl_engine::getJsonData<f32>( data_, "amplitude" ).value() },
                                     .frequency { sdl_engine::getJsonData<f32>( data_, "frequency" ).value() },
                                     .move_speed { sdl_engine::getJsonData<f32>( data_, "move_speed" ).value() },
                                     .move_threshold {
                                       sdl_engine::getJsonData<f32>( data_, "move_threshold" ).value() },
                                     .time { 0.0f } };
      return move_comp;
   }
}    // namespace myge
