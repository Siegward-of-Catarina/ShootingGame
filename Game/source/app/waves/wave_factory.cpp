#include <pch.hpp>
// my header
#include <app/waves/wave_factory.hpp>
// wave
#include <app/waves/basic_wave.hpp>
#include <app/waves/wave.hpp>
namespace myge
{
   std::unique_ptr<Wave> createWave( std::string_view waveType, WaveDependencies& dependencies_ )
   {
      if ( waveType == "BasicWave" ) return std::make_unique<BasicWave>( dependencies_ );
      return nullptr;
   }
}    // namespace myge
