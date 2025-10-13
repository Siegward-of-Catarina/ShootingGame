#include <app/waves/wave.hpp>
namespace myge
{
   Wave::Wave( WaveDependencies& dependencies_ )
     : _wave_data {}
     , _wave_end { false }
     , _registry { dependencies_.registry }
     , _resource_manager { dependencies_.resource_manager }
   {
   }
   Wave::~Wave() {}
   void Wave::loadWaveData( const std::string& wave_name_ ) { _wave_data = sdl_engine::loadJson( wave_name_ ); }
}    // namespace myge