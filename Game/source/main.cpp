#include "game.hpp"

#include <SDL3/SDL_main.h>
int main( int argc, char* argv[] )
{
   myge::Game game;
   if ( game.init() ) { game.run(); }
   game.quit();
   return 0;
}