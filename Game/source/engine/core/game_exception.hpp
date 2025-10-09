#pragma once
#include <stdexcept>
namespace sdl_engine
{
   class GameException : public std::runtime_error
   {
   public:
      GameException( const char* message_ ) : std::runtime_error( message_ ) {}
   };

}    // namespace sdl_engine