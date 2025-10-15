#include <SDL3/SDL.h>
#include <engine/core.hpp>
#include <engine/core/game_context.hpp>
#include <engine/rendering/renderer.hpp>
// basic systems
#include <engine/basic_system.hpp>
namespace sdl_engine
{
   EngineContext::EngineContext( std::string_view window_name_, i32 window_width_, i32 window_height_ )
     : _window { nullptr, nullptr }
     , _renderer { nullptr }
     , _scene_manager { nullptr }
     , _resource_manager { nullptr }
     , _input_manager { nullptr }
     , _window_size { window_width_, window_height_ }
     , _system_manager { nullptr }
     , _game_timer { std::make_unique<GameTimer>() }
   {
      if ( !SDL_Init( SDL_INIT_VIDEO ) ) { throw GameException( "SDLの初期化に失敗しました" ); }

#ifdef _DEBUG
      SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INVALID );
#endif
      // window, renderer 取得
      // いったん生ポインタで取得し、unique_ptrへ
      SDL_Window* window_raw = SDL_CreateWindow( window_name_.data(), _window_size.x, _window_size.y, 0 );
      if ( !window_raw ) { throw GameException( "SDLwindowを作成できませんでした" ); }

      // uniqueへ代入
      _window = { window_raw, &SDL_DestroyWindow };

      _renderer = std::make_unique<Renderer>( window_raw );

      _resource_manager = std::make_unique<ResourceManager>();

      _input_manager = std::make_unique<InputManager>();

      _scene_manager = std::make_unique<SceneManager>( _dispatcher );

      _system_manager = std::make_unique<SystemManager>( _registry, *_renderer );
   }
   EngineContext::~EngineContext() {}

   void EngineContext::update()
   {
      _game_timer->update();
      _scene_manager->update( *this );
      _system_manager->updateSystems( framgeData() );
      _renderer->renderPresent();
      _input_manager->update();
   }
   void EngineContext::loadAssets( std::string_view assets_path_ )
   {
      _resource_manager->loadResources( *_renderer, assets_path_.data() );
   }
   FrameData EngineContext::framgeData()
   {
      return { _game_timer->getDeltaTime(), static_cast<f32>( _window_size.x ), static_cast<f32>( _window_size.y ) };
   }
}    // namespace sdl_engine
