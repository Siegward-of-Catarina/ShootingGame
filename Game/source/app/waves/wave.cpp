#include <app/waves/wave.hpp>
namespace myge
{
	Wave::Wave() : _wave_data{}, _wave_end{false} {}
   Wave::~Wave() {}
   void Wave::loadWaveData(const std::string& wave_name_)
   {
	   _wave_data = sdl_engine::loadJson(wave_name_);
   }
}    // namespace myge