#include <engine/components/transform.hpp>

namespace sdl_engine
{
   // JSON 値が無い場合は既定値を適用
   Transform createTransform( const json& data_ )
   {
      Transform trfm_comp {
         .position { data_.value( "px", 0.0f ), data_.value( "py", 0.0f ) },
         .angle { data_.value( "angle", 0.0f ) },
         .scale { data_.value( "scale", 1.0f ) }
      };
      return trfm_comp;
   }

   Transform createTransform()
   {
      return Transform {
         { 0.0f, 0.0f },
         0.0f, 1.0f
      };
   }
}    // namespace sdl_engine