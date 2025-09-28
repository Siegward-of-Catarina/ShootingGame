#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <engine/math/vector4.hpp>
#include <engine/rendering/renderer.hpp>
#include <stdexcept>
namespace sdl_engine
{
   Renderer::Renderer() : _sdl_renderer { nullptr, nullptr } {}
   Renderer::Renderer( SDL_Window* window_ ) : _sdl_renderer { nullptr, nullptr }
   {

      SDL_Renderer* renderer_raw = SDL_CreateRenderer( window_, nullptr );
      if ( !renderer_raw ) { SDL_Log( "SDLrendererÇçÏê¨Ç≈Ç´Ç‹ÇπÇÒÇ≈ÇµÇΩ: %s", SDL_GetError() ); }

      _sdl_renderer = { renderer_raw, &SDL_DestroyRenderer };
   }
   Renderer::~Renderer() {}

   SDL_Texture* Renderer::LoadTexture( std::string_view path )
   {
      return IMG_LoadTexture( _sdl_renderer.get(), path.data() );
   }

   void Renderer::setRenderClearColor( float r_, float g_, float b_, float a_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), r_, g_, b_, a_ );
   }
   void Renderer::setRenderClearColor( Color& color_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r, color_.g, color_.b, color_.a );
   }

   void Renderer::RenderClear() { SDL_RenderClear( _sdl_renderer.get() ); }
   void Renderer::RenderClear( Color color_ )
   {

      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), color_.r, color_.g, color_.b, color_.a );
      SDL_RenderClear( _sdl_renderer.get() );
   }
   void Renderer::RenderClear( float r_, float g_, float b_, float a_ )
   {
      SDL_SetRenderDrawColorFloat( _sdl_renderer.get(), r_, g_, b_, a_ );
      SDL_RenderClear( _sdl_renderer.get() );
   }

   void Renderer::RenderTexture( SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_ )
   {
      SDL_RenderTextureRotated(
        _sdl_renderer.get(), texture_, src_, dst_, static_cast<f64>( angle_ ), nullptr, SDL_FLIP_NONE );
   }

   void Renderer::RenderPresent() { SDL_RenderPresent( _sdl_renderer.get() ); }
}    // namespace sdl_engine
