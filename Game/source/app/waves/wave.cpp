#include <app/waves/wave.hpp>
namespace myge
{
	Wave::Wave() : _wave_data{}, _active_time{0.0} {}
   Wave::~Wave() {}
   void Wave::loadWaveData(const std::string& wave_name_)
   {
	   _wave_data = sdl_engine::loadJson(wave_name_);
	   auto common = _wave_data.at("WaveCommon");
	   _active_time = common.at("active_time");
   }
}    // namespace myge