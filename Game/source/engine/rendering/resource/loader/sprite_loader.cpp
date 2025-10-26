#include <SDL3/SDL.h>
#include <engine/rendering/renderer.hpp>
#include <engine/rendering/resource/loader/sprite_loader.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <filesystem>
namespace sdl_engine
{
   SpriteLoader::result_type SpriteLoader::operator()( Renderer& renderer_, const json& data_ ) const
   {
      auto path_ = getRequireData<std::string>( data_, "file_path" );
      if ( !std::filesystem::exists( path_ ) )
      {
         std::string msg { "画像ファイルが存在しません: " };
         msg += path_.data();
         throw GameException( msg.c_str() );
      }

      auto out { std::make_shared<SpriteResource>( renderer_.loadTexture( path_ ) ) };

      SDL_SetTextureBlendMode( out->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( out->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return out;
   }
   SpriteLoader::result_type SpriteLoader::operator()( SDL_Texture*& texture_ ) const
   {
      auto out { std::make_shared<SpriteResource>( texture_ ) };
      texture_ = nullptr;

      SDL_SetTextureBlendMode( out->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( out->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return out;
   }
}    // namespace sdl_engine