#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <engine/core.hpp>
#include <engine/core/game_context.hpp>
#include <engine/events/event_listener.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/sound/sound_mixer.hpp>
// basic systems
#include <engine/basic_system.hpp>
// event
#include <engine/events/quit_event.hpp>
#include <engine/utils/common_utilities.hpp>

namespace sdl_engine
{
   // エンジン初期化：SDL/Window/Renderer/Sound/Managers を構築
   EngineContext::EngineContext( EngineInitInfo& info_ )
     : _window { nullptr, nullptr }
     , _dispatcher {}
     , _registry {}
     , _window_size { info_.window_width, info_.window_height }
     , _renderer { nullptr }
     , _soundmixer { nullptr }
     , _scene_manager { nullptr }
     , _resource_manager { nullptr }
     , _input_manager { nullptr }
     , _system_manager { nullptr }
     , _event_listener { nullptr }
     , _game_timer { nullptr }
     , is_quit { false }
   {
      if ( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) ) { throw GameException( "SDLの初期化に失敗しました" ); }

#ifdef _DEBUG
      SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INVALID );
#endif
      // Window/Renderer 構築
      SDL_Window* window_raw = SDL_CreateWindow( info_.window_name.data(), _window_size.x, _window_size.y, 0 );
      if ( !window_raw ) { throw GameException( "SDLwindowを作成できませんでした" ); }
      _window = { window_raw, &SDL_DestroyWindow };

      _renderer         = std::make_unique<Renderer>( window_raw );
      _soundmixer       = std::make_unique<SoundMixer>( info_.se_track_size );
      _resource_manager = std::make_unique<ResourceManager>();
      _input_manager    = std::make_unique<InputManager>();
      _game_timer       = std::make_unique<GameTimer>();
      _event_listener   = std::make_unique<EventListener>( _dispatcher );
      _system_manager   = std::make_unique<SystemManager>( _registry, *_renderer, *_soundmixer, *_event_listener );
      _scene_manager    = std::make_unique<SceneManager>( _dispatcher, *_game_timer );

      // Quit イベントを購読
      _event_listener->connect<&EngineContext::onQuitEvent, QuitEvent>( this );
   }

   EngineContext::~EngineContext()
   {
      // SDL_Quit 前にリソースを明示解放
      _resource_manager.reset();
      _soundmixer.reset();
      _renderer.reset();
      SDL_Quit();
   }

   void EngineContext::update()
   {
      // 1フレーム処理の順序
      _game_timer->update();
      _scene_manager->update( framgeData() );
      _system_manager->updateSystems( framgeData() );
      _renderer->renderPresent();
      _input_manager->update();
   }

   void EngineContext::loadAssets( std::string_view assets_path_ )
   {
      _resource_manager->loadResources( *_renderer, *_soundmixer, assets_path_.data() );
   }

   SceneDependencies EngineContext::getSceneDependencies()
   {
      return SceneDependencies { .registry { _registry },
                                 .dispatcher { _dispatcher },
                                 .resource_manager { *_resource_manager },
                                 .input_manager { *_input_manager },
                                 .scene_manager { *_scene_manager },
                                 .system_manager { *_system_manager } };
   }

   FrameData EngineContext::framgeData()
   {
      return { _game_timer->getDeltaTime(), static_cast<f32>( _window_size.x ), static_cast<f32>( _window_size.y ) };
   }

   void EngineContext::onQuitEvent( [[maybe_unused]] QuitEvent& e ) { is_quit = true; }
}    // namespace sdl_engine