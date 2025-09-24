#pragma once
#include <stdexcept>
namespace myge
{
   class GameException : public std::runtime_error
   {
   public:
      GameException( const char* message_ ) : std::runtime_error( message_ ) {}
   };

}    // namespace myge