#include <SDL3/SDL.h>
#include <engine/core/game_context.hpp>
#include <engine/managers/input_manager.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/scene/scene.hpp>
namespace sdl_engine
{
   GameContext::GameContext( std::string_view window_name_, i32 window_width_, i32 window_height_ )
     : _window { nullptr, nullptr }
     , _renderer { nullptr }
     , _sequencer { std::make_unique<SceneManager>() }
     , _resource_manager { std::make_unique<ResourceManager>() }
     , _input_manager { std::make_unique<InputManager>() }
     , _window_size { window_width_, window_height_ }
   {
      if ( !SDL_Init( SDL_INIT_VIDEO ) ) { throw GameException( "SDLの初期化に失敗しました" ); }

      // window, renderer 取得
      // いったん生ポインタで取得し、unique_ptrへ
      SDL_Window* window_raw = SDL_CreateWindow( window_name_.data(), _window_size.x, _window_size.y, 0 );
      if ( !window_raw ) { throw GameException( "SDLwindowを作成できませんでした" ); }

      // uniqueへ代入
      _window = { window_raw, &SDL_DestroyWindow };

      _renderer = std::make_unique<Renderer>( window_raw );
   }
   GameContext::~GameContext() {}
   void GameContext::setNextScene( std::unique_ptr<Scene> next_ ) { _sequencer->setNextScene( std::move( next_ ) ); }
   void GameContext::setCurrentScene( Scene* const current_ ) { _sequencer->setCurrentScene( current_ ); }

}    // namespace sdl_engine