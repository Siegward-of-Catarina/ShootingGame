#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <engine/core/game_exception.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/rendering/resource/color.hpp>
#include <engine/Components/text.hpp>
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
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r(), color_.g(), color_.b(), color_.a() );
   }

   void Renderer::renderClear() { SDL_RenderClear( _sdl_renderer.get() ); }
   void Renderer::renderClear( ColorRGBA& color_ )
   {

      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r(), color_.g(), color_.b(), color_.a() );
      SDL_RenderClear( _sdl_renderer.get() );
   }
   void Renderer::renderClear( float r_, float g_, float b_, float a_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), r_, g_, b_, a_ );
      SDL_RenderClear( _sdl_renderer.get() );
   }

   void Renderer::renderSetDebugText( std::string_view text_, f32 x_, f32 y_ )
   {
       std::string text{ text_ };
       //_text_render_queue.emplace( Text{ text, Text::Size::Small, ColorRGBA(1.0f,1.0f,0.0f,1.0f) } );

   }

   void Renderer::renderSetText(Text& text_)
   {
       _text_render_queue.emplace(text_);
   }

   void Renderer::renderTexture( SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_ )
   {
      SDL_RenderTextureRotated(
        _sdl_renderer.get(), texture_, src_, dst_, static_cast<f64>( angle_ ), nullptr, SDL_FLIP_NONE );
   }

   void Renderer::renderPresent() { SDL_RenderPresent( _sdl_renderer.get() ); }
}    // namespace sdl_engine
