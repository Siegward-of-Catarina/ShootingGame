#include <engine/utils/common_utilities.hpp>
#include <exception>

namespace sdl_engine
{
   void throwIfError( bool condition_, const std::string_view message_ )
   {
      if ( !condition_ ) { throw std::runtime_error( message_.data() ); }
   }
}    // namespace sdl_engine
