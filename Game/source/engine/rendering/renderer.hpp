#pragma once
#include <engine/core/forward_declarations.hpp>

using SDL_RendererPtr = std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )>;

namespace sdl_engine
{
   // SDL_Rendererラップクラス
   class Renderer
   {
   public:
      Renderer();
      Renderer( SDL_Window* window_ );
      ~Renderer();

      SDL_Texture* LoadTexture( std::string_view path );

      void setRenderClearColor( float r_, float g_, float b_, float a_ );
      void setRenderClearColor( Color& color_ );

      void RenderClear();
      void RenderClear( Color color_ );
      void RenderClear( float r_, float g_, float b_, float a_ );

      void RenderTexture( SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_ );

      void RenderPresent();

   private:
      SDL_RendererPtr _sdl_renderer;
   };
}    // namespace sdl_engine
