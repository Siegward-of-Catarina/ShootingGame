#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   struct SpriteResource
   {
      SpriteResource() : texture { nullptr } {}
      SpriteResource( SDL_Texture* texture_ ) : texture { texture_ } {}
      ~SpriteResource() { SDL_DestroyTexture( texture ); };
      SDL_Texture* texture;
   };
}    // namespace sdl_engine