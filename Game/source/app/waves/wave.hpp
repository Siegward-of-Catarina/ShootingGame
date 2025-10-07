#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace myge
{
   class Wave
   {
   public:
      Wave();
      virtual ~Wave();
      virtual void start( sdl_engine::GameContext& context_ )  = 0;
      virtual void update( sdl_engine::GameContext& context_ ) = 0;
      void         loadWaveData( const std::string& wave_name_ );
      bool         isWaveEnd() { return _wave_end; }

   protected:
       json& getWaveData() { return _wave_data; }
       void endWave() { _wave_end = true; }
   private:
      json _wave_data;
      bool _wave_end;
   };
}    // namespace myge
