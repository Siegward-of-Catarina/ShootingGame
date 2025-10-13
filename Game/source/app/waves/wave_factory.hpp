#pragma once
namespace myge
{
   struct WaveDependencies;
   class Wave;
   std::unique_ptr<Wave> createWave( std::string_view waveType, WaveDependencies& dependencies_ );
}    // namespace myge