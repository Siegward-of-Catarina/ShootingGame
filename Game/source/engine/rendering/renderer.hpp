#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <engine/components/text.hpp>
#include <engine/core/forward_declarations.hpp>
using SDL_RendererPtr = std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )>;
namespace sdl_engine
{

   // rendererに渡す用の transform　componentのtransformに依存したくない
   struct RenderTransform
   {
      f32 x;
      f32 y;
      f32 angle;
      f32 scale;
   };
   // SDL_Rendererラップクラス
   class Renderer
   {
   public:
      Renderer();
      Renderer( SDL_Window* window_ );
      ~Renderer();

      SDL_Texture* loadTexture( std::string_view path );
      SDL_Texture* createWhiteTexture();

      void setRenderClearColor( float r_, float g_, float b_, float a_ );
      void setRenderClearColor( ColorRGBA& color_ );

      void renderClear();
      void renderClear( ColorRGBA& color_ );
      void renderClear( float r_, float g_, float b_, float a_ );
      void renderText( SDL_Texture*           texture_,
                       const FontResource&    font_,
                       const std::string&     text_,
                       const RenderTransform& trfm_,
                       const ColorRGBA&       color_ );
      void renderTexture( SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_ );

      void renderPresent();

   private:
      SDL_RendererPtr  _sdl_renderer;
      std::queue<Text> _text_render_queue;
   };
}    // namespace sdl_engine
