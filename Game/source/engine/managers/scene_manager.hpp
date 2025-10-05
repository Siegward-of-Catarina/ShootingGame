#pragma once
#include <engine/forward.hpp>
namespace sdl_engine
{
   class SceneManager
   {
   public:
      SceneManager();
      ~SceneManager();
      void   initialize( GameContext& context_ );
      void   update( GameContext& context_ );
      void   initializeCurrentScene( std::unique_ptr<Scene> current_ );
      void   setNextScene( std::unique_ptr<Scene> next_ ) { _next_scene = std::move( next_ ); };
      Scene& currentScene() { return *_current_scene; };

      // メモ：シーンの遷移について
      // シーンが終了時に、シーン自身にNextSceneをsetさせる。
      // nextsceneは通常nullptrであり、setされたときのみアドレスを持つ。
      // シーンマネージャはこのnextsceneを監視し、nullptrでない場合、
      // フェードアウトを実行する。
      // フェードアウトが完了し次第、currentにnextを移動させ、nextはnullptrに。
      // その後、sceneの初期化が終わり次第フェードインを実行する。

   private:
      entt::entity           _fade;
      std::unique_ptr<Scene> _current_scene;
      std::unique_ptr<Scene> _next_scene;
   };
}    // namespace sdl_engine