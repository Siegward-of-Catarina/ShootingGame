#include <pch.hpp>
// my header
#include <engine/utils/common_utilities.hpp>
// exception
#include <engine/core/game_exception.hpp>

namespace sdl_engine
{
   void throwIfError( bool condition_, const std::string_view message_ )
   {
      if ( !condition_ ) { throw std::runtime_error( std::string( message_ ) ); }
   }
}    // namespace sdl_engine
