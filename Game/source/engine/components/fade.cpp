#include <engine/components/fade.hpp>

namespace sdl_engine
{

   Fade createFade( const json& data_ )
   {
      Fade fade_comp { Fade::State::Idle, Fade::Type::OutIn };
      fade_comp.end_alpha          = getOptionalData<f32>( data_, "end_alpha", 0.0f );
      fade_comp.target_out_alpha   = getOptionalData<f32>( data_, "target_out_alpha", 1.0f );
      fade_comp.speed              = getOptionalData<f32>( data_, "speed", 2.0f );
      fade_comp.black_out_duration = getOptionalData<f32>( data_, "black_out_duration", 1.0f );
      auto type { getRequireData<std::string>( data_, "type" ) };
      if ( type == "OutIn" ) { fade_comp.type = Fade::Type::OutIn; }
      else if ( type == "In" ) { fade_comp.type = Fade::Type::In; }
      else if ( type == "Out" ) { fade_comp.type = Fade::Type::Out; }
      return fade_comp;
   }
   Fade createFade() { return Fade { Fade::State::Idle, Fade::Type::OutIn, 0.0f, 1.0f, 2.0f, 0.0f, 1.0f }; }
}    // namespace sdl_engine