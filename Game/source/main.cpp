#include <SDL3/SDL_main.h>
#include <core/game_exception.hpp>
#include <game.hpp>
#include <iostream>
int main( int argc, char* argv[] )
{

   try
   {
      myge::Game game;
      game.run();
   }
   catch ( const myge::GameException& e )
   {
      std::cerr << "Game Exception: " << e.what() << '\n';
   }
   catch ( const std::exception& e )
   {
      std::cerr << "Standard Exception: " << e.what() << '\n';
   }
   catch ( ... )
   {
      std::cerr << "Unknown Exception" << '\n';
   }

   return 0;
}