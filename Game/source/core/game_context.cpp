#include "game_context.hpp"

#include <SDL3/SDL.h>
namespace myge
{
   GameContext::GameContext()
     : _window { nullptr, nullptr }
     , _renderer { nullptr }
     , _sequencer { std::make_unique<Sequencer>() }
     , _resource_manager { std::make_unique<ResourceManager>() }
   {
      // �����[�Ƃ��g������O���o����
      if ( !SDL_Init( SDL_INIT_VIDEO ) ) { SDL_Log( "SDL���������ł��܂���ł���:  %s ", SDL_GetError() ); }
   }
   GameContext::~GameContext() {}
}    // namespace myge