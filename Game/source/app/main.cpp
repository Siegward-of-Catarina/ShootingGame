#include <pch.hpp>
// sdl main
#include <SDL3/SDL_main.h>
// game
#include <app/game.hpp>
// core
#include <engine/core.hpp>
// stl
#include <iostream>
// SDL_mainを使用する場合 argc, argvをmainで受け取る必要があるが、使用しないのでこの形をとる
int main( [[maybe_unused]] int argc, [[maybe_unused]] char* argv[] )
{
   try
   {
      myge::Game game {};
      game.run();
   }
   catch ( const sdl_engine::GameException& e )
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