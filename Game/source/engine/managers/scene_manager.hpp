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
      SceneManager( entt::dispatcher& dispatcher_, GameTimer& game_timer_ );
      ~SceneManager();
      void update( const FrameData& frame_ );
      // 最初期シーン初期化後開始
      void initStartCurrentScene( std::unique_ptr<Scene> current_ );

      void quitGame();

      void   setNextScene( std::unique_ptr<Scene> next_ ) { _next_scene = std::move( next_ ); };
      void   enableFadeOutIn( entt::entity fade_ ) { _fade = fade_; };
      Scene& currentScene() { return *_current_scene; };

      void setGameSpeed( const f32 speed_ ) { _game_timer.setGameSpeed( speed_ ); };
      // 追加: フェードエンティティの取得（シーンから制御したい場合に使用）
      std::optional<entt::entity> fadeEntity() const { return _fade; }

   private:
      // フェードシステムからのイベントコールバック
      void onFadeOutEnd( FadeOutEndEvent& e );
      void onFadeInEnd( FadeInEndEvent& e );

   private:
      GameTimer&                     _game_timer;
      std::unique_ptr<EventListener> _event_listener;
      std::optional<entt::entity>    _fade;
      std::unique_ptr<Scene>         _current_scene;
      std::unique_ptr<Scene>         _next_scene;
      bool                           _enable_update;
   };
}    // namespace sdl_engine