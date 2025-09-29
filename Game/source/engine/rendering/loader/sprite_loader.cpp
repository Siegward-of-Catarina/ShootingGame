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
      if ( !std::filesystem::exists( path_ ) ) { SDL_Log( "‰æ‘œƒtƒ@ƒCƒ‹‚ª‘¶Ý‚µ‚Ü‚¹‚ñ:  %s ", path_.data() ); }

      SpriteResource* resource { new SpriteResource };
      resource->depth   = data_.value( "depth", 0 );
      resource->texture = renderer_.LoadTexture( path_ );
      if ( !resource->texture ) { SDL_Log( "‰æ‘œ‚ð“Ç‚Ýž‚ß‚Ü‚¹‚ñ‚Å‚µ‚½:  %s ", SDL_GetError() ); }

      SDL_SetTextureScaleMode( resource->texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return std::shared_ptr<SpriteResource>( resource, SpriteResourceDeleter );
   }
}    // namespace sdl_engine