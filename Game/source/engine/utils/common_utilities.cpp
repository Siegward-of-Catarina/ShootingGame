#include <engine/core/game_exception.hpp>
#include <engine/utils/common_utilities.hpp>

namespace sdl_engine
{
   void throwIfError( bool condition_, const std::string_view message_ )
   {
      if ( !condition_ ) { throw GameException( message_.data() ); }
   }
}    // namespace sdl_engine
