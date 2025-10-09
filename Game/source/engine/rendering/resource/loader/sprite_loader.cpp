#include <SDL3/SDL.h>
#include <engine/rendering/renderer.hpp>
#include <engine/rendering/resource/loader/sprite_loader.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <filesystem>
namespace sdl_engine
{
   SpriteLoader::result_type SpriteLoader::operator()( Renderer& renderer_, const json& data_ ) const
   {
      auto path_ = data_.at( "file_path" ).get<std::string>();
      if ( !std::filesystem::exists( path_ ) ) { SDL_Log( "画像ファイルが存在しません:  %s ", path_.data() ); }

      SpriteResource* resource { new SpriteResource };
      resource->texture = renderer_.loadTexture( path_ );
      if ( !resource->texture ) { SDL_Log( "画像を読み込めませんでした:  %s ", SDL_GetError() ); }

      SDL_SetTextureBlendMode( resource->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( resource->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return std::shared_ptr<SpriteResource>( resource, SpriteResourceDeleter );
   }
   SpriteLoader::result_type SpriteLoader::operator()( SDL_Texture* texture_ ) const
   {
      SpriteResource* resource { new SpriteResource };
      resource->texture = texture_;

      SDL_SetTextureBlendMode( resource->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( resource->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return std::shared_ptr<SpriteResource>( resource, SpriteResourceDeleter );
   }
}    // namespace sdl_engine