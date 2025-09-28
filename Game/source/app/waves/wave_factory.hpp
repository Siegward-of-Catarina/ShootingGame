#pragma once
namespace myge
{
   class Wave;
   std::unique_ptr<Wave> createWave( std::string_view waveType );
}    // namespace myge