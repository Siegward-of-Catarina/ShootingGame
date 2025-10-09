#include <app/waves/wave.hpp>
#include <app/waves/wave1.hpp>
#include <app/waves/wave_factory.hpp>
namespace myge
{
   std::unique_ptr<Wave> createWave( std::string_view waveType )
   {
      if ( waveType == "Wave1" ) return std::make_unique<Wave1>();
      return nullptr;
   }
}    // namespace myge
