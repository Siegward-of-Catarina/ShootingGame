#pragma once
#include <engine/forward.hpp>
#include <optional>
namespace sdl_engine
{
   struct FadeOutStartEvent;
   struct FadeOutEndEvent;
   struct FadeInEndEvent;

   class SceneManager
   {
   public:
      SceneManager( entt::dispatcher& dispatcher_ );
      ~SceneManager();
      void update( EngineContext& context_ );
      // 最初期シーン初期化後開始
      void   initStartCurrentScene( std::unique_ptr<Scene> current_ );
      void   setNextScene( std::unique_ptr<Scene> next_ ) { _next_scene = std::move( next_ ); };
      void   enableFadeOutIn( entt::entity fade_ ) { _fade = fade_; };
      Scene& currentScene() { return *_current_scene; };

   private:
      // フェードシステムからのイベントコールバック
      void onFadeOutEnd( FadeOutEndEvent& e );
      void onFadeInEnd( FadeInEndEvent& e );

   private:
      entt::dispatcher&           _dispatchar;
      std::optional<entt::entity> _fade;
      std::unique_ptr<Scene>      _current_scene;
      std::unique_ptr<Scene>      _next_scene;
      bool                        _enable_update;
   };
}    // namespace sdl_engine