#include <engine/components/velocity.hpp>
namespace sdl_engine
{
   Velocity createVelocity( const json& data_ )
   {
      Velocity vlcy_comp {
         .vector { data_.value( "dx", 0.0f ), data_.value( "dy", 0.0f ) },
         .anguler { data_.value( "anguler", 0.0f ) },
         .scale_rate { data_.value( "scale_rate", 0.0f ) }
      };
      return vlcy_comp;
   }
}    // namespace sdl_engine