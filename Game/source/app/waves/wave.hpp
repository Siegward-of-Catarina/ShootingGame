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
      void         loadWaveData( const std::string& wave_name_ ) { _wave_data = sdl_engine::loadJson( wave_name_ ); }
      virtual void start( entt::registry& registry_, sdl_engine::GameContext& context_ )              = 0;
      virtual void update( entt::registry& registry_, sdl_engine::GameContext& context_, f32 delta_ ) = 0;

   protected:
      json _wave_data;
   };
}    // namespace myge
