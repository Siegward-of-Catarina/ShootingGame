#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   struct SoundResource
   {
      SoundResource() : audio { nullptr } {}
      SoundResource( MIX_Audio* audio_ ) : audio { audio_ } {}
      ~SoundResource() { MIX_DestroyAudio( audio ); }
      MIX_Audio* audio;
   };
}    // namespace sdl_engine