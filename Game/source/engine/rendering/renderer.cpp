#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <engine/Components/text.hpp>
#include <engine/core/game_exception.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/rendering/resource/color.hpp>
#include <engine/rendering/resource/font_resource.hpp>
namespace sdl_engine
{
   Renderer::Renderer() : _sdl_renderer { nullptr, nullptr } {}
   Renderer::Renderer( SDL_Window* window_ ) : _sdl_renderer { nullptr, nullptr }
   {

      SDL_Renderer* renderer_raw = SDL_CreateRenderer( window_, nullptr );
      if ( !renderer_raw )
      {
         std::string msg = "SDL rendererを作成できませんでした: " + std::string( SDL_GetError() );
         throw GameException( msg.c_str() );
      }
      _sdl_renderer = { renderer_raw, &SDL_DestroyRenderer };
   }
   Renderer::~Renderer() {}

   SDL_Texture* Renderer::loadTexture( std::string_view path )
   {

      auto texture { IMG_LoadTexture( _sdl_renderer.get(), path.data() ) };
      if ( !texture )
      {
         std::string msg = "SDL_Textureをロードに失敗しました: " + std::string( SDL_GetError() );
         throw GameException( msg.c_str() );
      }

      SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return texture;
   }

   SDL_Texture* Renderer::createWhiteTexture()
   {
      // create white surface 1x1
      auto surface { SDL_CreateSurface( 1, 1, SDL_PixelFormat::SDL_PIXELFORMAT_RGBA8888 ) };
      SDL_FillSurfaceRect( surface, nullptr, SDL_MapSurfaceRGBA( surface, 255, 255, 255, 255 ) );
      // create texture
      auto texture { SDL_CreateTextureFromSurface( _sdl_renderer.get(), surface ) };
      if ( !texture )
      {
         std::string msg = "SDL_Textureを作成できませんでした: " + std::string( SDL_GetError() );
         throw GameException( msg.c_str() );
      }

      SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );    // 標準はアルファブレンド
      SDL_SetTextureScaleMode( texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST );

      return texture;
   }

   void Renderer::setRenderClearColor( float r_, float g_, float b_, float a_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), r_, g_, b_, a_ );
   }
   void Renderer::setRenderClearColor( ColorRGBA& color_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r, color_.g, color_.b, color_.a );
   }

   void Renderer::renderClear() { SDL_RenderClear( _sdl_renderer.get() ); }
   void Renderer::renderClear( ColorRGBA& color_ )
   {

      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r, color_.g, color_.b, color_.a );
      SDL_RenderClear( _sdl_renderer.get() );
   }
   void Renderer::renderClear( float r_, float g_, float b_, float a_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), r_, g_, b_, a_ );
      SDL_RenderClear( _sdl_renderer.get() );
   }

   void Renderer::renderText( SDL_Texture*           texture_,
                              const FontResource&    font_,
                              const std::string&     text_,
                              const RenderTransform& trfm_,
                              const ColorRGBA&       color_ )
   {
      if ( !texture_ ) { return; }

      SDL_SetTextureColorModFloat( texture_, color_.r, color_.g, color_.b );
      SDL_SetTextureAlphaModFloat( texture_, color_.a );

      const f32 char_w = font_.width;
      const f32 char_h = font_.height;
      const f32 half_w = char_w * 0.5f;
      const f32 half_h = char_h * 0.5f;

      f32 base_x = trfm_.x;
      f32 base_y = trfm_.y;

      u32 col = 0, row = 0;
      for ( u8 c : text_ )
      {
         if ( c == '\n' )
         {
            ++row;
            col = 0;
            continue;
         }
         const u32 idx { static_cast<u32>( c ) - static_cast<u32>( ' ' ) };
         if ( idx >= font_.font_rect.size() )
         {
            // 範囲外の文字はスキップ（必要ならフォールバック文字を使う）
            ++col;
            continue;
         }

         SDL_FRect src {};
         SDL_FRect dst {};
         src   = font_.font_rect[ idx ];
         dst.w = char_w;
         dst.h = char_h;

         dst.x = base_x + ( char_w * static_cast<f32>( col++ ) ) - half_w;
         dst.y = base_y + ( char_h * static_cast<f32>( row ) ) - half_h;
         renderTexture( texture_, &src, &dst, trfm_.angle );
      }
   }
   void Renderer::renderTexture( SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_ )
   {
      SDL_RenderTextureRotated(
        _sdl_renderer.get(), texture_, src_, dst_, static_cast<f64>( angle_ ), nullptr, SDL_FLIP_NONE );
   }

   void Renderer::renderPresent() { SDL_RenderPresent( _sdl_renderer.get() ); }
}    // namespace sdl_engine
