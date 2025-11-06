#include <pch.hpp>
// my header
#include <engine/rendering/resource/loader/sprite_loader.hpp>
// resource
#include <engine/rendering/resource/sprite_resource.hpp>
// render
#include <engine/rendering/renderer.hpp>
// stl
#include <filesystem>
namespace sdl_engine
{
   SpriteLoader::result_type SpriteLoader::operator()( Renderer& renderer_, const json& data_ ) const
   {
      auto path = getRequireData<std::string>( data_, "file_path" );
      if ( !std::filesystem::exists( path ) )
      {
         std::string msg { "image file does not exist : " + path };
         throw GameException( msg.c_str() );
      }

      auto out { std::make_shared<SpriteResource>( renderer_.loadTexture( path ) ) };

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