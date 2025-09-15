#include "test_scene.hpp"

#include "game.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
namespace
{
   uint32_t frame { 0 };
   uint32_t max_frame { 8 };
   float    x { 200 };
   float    y { 800 };

}    // namespace
namespace myge
{
   TestScene::TestScene( Game* game_ ) : Scene( game_ )
   {
      image = IMG_LoadTexture( game()->render(), "resource/image/enemy01_sprite.png" );
      if ( image == nullptr ) { SDL_Log( "画像を読み込めませんでした:  %s ", SDL_GetError() ); }
      size_t length { 8 };
      for ( size_t i = 0; i < length; i++ ) { sprite.emplace_back( SDL_FRect( 32 * i, 0, 32, 40 ) ); }
   }
   TestScene::~TestScene() {}
   void myge::TestScene::proc( float _delta )
   {
      SDL_SetRenderDrawColorFloat( game()->render(), 0.3f, 0.3f, 0.3f, 1.f );
      frame += _delta * 1000;
      auto idx = frame / 30 % 8;
      y -= 100.f * _delta;
      SDL_FRect drect { x, y, 32, 40 };
      if ( y < -40.f ) { y = 820; }
      SDL_RenderClear( game()->render() );
      SDL_RenderTexture( game()->render(), image, &sprite[ idx ], &drect );
      SDL_RenderPresent( game()->render() );
   }
}    // namespace myge
