#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   struct SpriteResource
   {
      SDL_Texture* texture;
      // �\���������߂�l�B0���őO��
      i32 depth;
   };
   inline void SpriteResourceDeleter( SpriteResource* sprite_ )
   {
      if ( sprite_ )
      {
         if ( sprite_->texture ) { SDL_DestroyTexture( sprite_->texture ); }
         delete sprite_;
      }
   }
}    // namespace sdl_engine