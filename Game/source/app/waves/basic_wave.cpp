#include <pch.hpp>
// my header
#include <app/waves/basic_wave.hpp>
// factory
#include <app/entity_factory.hpp>
// component
#include <app/components/affiliation.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace
{
   std::vector<entt::entity> entities;
}    // namespace
namespace myge
{
   BasicWave::BasicWave( WaveDependencies& dependencies_ ) : Wave { dependencies_ } {}

   void BasicWave::start( entt::entity player_ )
   {
      auto& wave_data { getWaveData() };
      if ( auto data { sdl_engine::tryGetJson( wave_data, "Entities" ) }; data )
      {
         EntityFactory factory { registry(), resourceManager() };
         auto          entts { factory.createEntities( *data, typeid( AffilGameScene ), player_ ) };
         entities = entts;
      }
   }
   void BasicWave::update( f32 )
   {
      i32 i { 0 };
      for ( auto& entity : entities )
      {
         if ( !registry().valid( entity ) ) { i++; }
         else { break; }
      }

      if ( i == entities.size() ) { endWave(); }
   }
}    // namespace myge
