#pragma once
#include <engine/forward.hpp>
namespace sdl_engine
{
   class SceneManager
   {
   public:
      SceneManager();
      ~SceneManager();
      // context廃止
      void initialize( EngineContext& context_ );
      void update( EngineContext& context_ );
      // 最初期シーン初期化後開始
      void   initStartCurrentScene( std::unique_ptr<Scene> current_ );
      void   setNextScene( std::unique_ptr<Scene> next_ ) { _next_scene = std::move( next_ ); };
      Scene& currentScene() { return *_current_scene; };

   private:
      entt::entity           _fade;
      std::unique_ptr<Scene> _current_scene;
      std::unique_ptr<Scene> _next_scene;
   };
}    // namespace sdl_engine