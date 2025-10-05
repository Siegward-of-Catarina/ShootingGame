#include <SDL3/SDL.h>
#include <engine/rendering/loader/sprite_loader.hpp>
#include <engine/rendering/loader/sprite_resource.hpp>
#include <engine/rendering/renderer.hpp>
#include <filesystem>
namespace sdl_engine
{
   std::shared_ptr<SpriteResource> SpriteLoader::operator()( Renderer& renderer_, const json& data_ ) const
   {
      auto path_ = data_.at( "file_path" ).get<std::string>();
      if ( !std::filesystem::exists( path_ ) ) { SDL_Log( "画像ファイルが存在しません:  %s ", path_.data() ); }

      SpriteResource* resource { new SpriteResource };
      resource->depth   = data_.value( "depth", 0 );
      resource->texture = renderer_.loadTexture( path_ );
      if ( !resource->texture ) { SDL_Log( "画像を読み込めませんでした:  %s ", SDL_GetError() ); }

      SDL_SetTextureBlendMode( resource->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( resource->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return std::shared_ptr<SpriteResource>( resource, SpriteResourceDeleter );
   }
   std::shared_ptr<SpriteResource> SpriteLoader::operator()( SDL_Texture* texture_, u32 depth_ ) const
   {
      SpriteResource* resource { new SpriteResource };
      resource->depth   = depth_;
      resource->texture = texture_;

      SDL_SetTextureBlendMode( resource->texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( resource->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return std::shared_ptr<SpriteResource>( resource, SpriteResourceDeleter );
   }
}    // namespace sdl_engine