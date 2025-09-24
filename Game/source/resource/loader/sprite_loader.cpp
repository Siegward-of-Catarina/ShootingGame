#include <SDL3/SDL.h>
#include <filesystem>
#include <renderer/renderer.hpp>
#include <resource/loader/sprite_loader.hpp>
namespace myge
{
   std::shared_ptr<SDL_Texture> SpriteLoader::operator()( Renderer& renderer_, const std::string_view path_ ) const
   {
      if ( !std::filesystem::exists( path_ ) ) { SDL_Log( "�摜�t�@�C�������݂��܂���:  %s ", path_.data() ); }

      auto image = renderer_.LoadTexture( path_ );
      if ( !image ) { SDL_Log( "�摜��ǂݍ��߂܂���ł���:  %s ", SDL_GetError() ); }

      return std::shared_ptr<SDL_Texture>( image, &SDL_DestroyTexture );
   }
}    // namespace myge