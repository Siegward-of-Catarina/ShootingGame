#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/scene/scene.hpp>
#include <engine/systems/fade_system.hpp>
namespace sdl_engine
{
   SceneManager::SceneManager() : _fade {}, _current_scene { nullptr }, _next_scene { nullptr } {}
   SceneManager::~SceneManager() {}
   void SceneManager::initialize( GameContext& context_ )
   {
      auto& renderer { context_.getRenderer() };
      auto& resource_manager { context_.getResourceManager() };

      // whiteテクスチャ作成
      auto white_texture { renderer.createWhiteTexture() };
      // 最前面 基本表示順は０からだが、絶対的に最前面に置くため-1とする
      resource_manager.addSpriteResources( "white", white_texture, -1 );

      auto& registry { context_.getRegistry() };
      auto& window_size { context_.getWindowSize() };

      _fade = registry.create();
      Transform trfm_cmp { .x { window_size.x / 2.0f }, .y { window_size.y / 2.0f }, .angle { 0 }, .scale { 1 } };
      registry.emplace<Transform>( _fade, trfm_cmp );

      Sprite sprt_comp {};
      sprt_comp.texture = context_.getResourceManager().getSprite( "white" );
      sprt_comp.src     = { 0, 0, static_cast<f32>( window_size.x ), static_cast<f32>( window_size.y ) };
      sprt_comp.dst = { trfm_cmp.x, trfm_cmp.y, static_cast<f32>( window_size.x ), static_cast<f32>( window_size.y ) };
      sprt_comp.color = { 0.0f, 0.0f, 0.0f, 0.0f };
      registry.emplace<Sprite>( _fade, sprt_comp );

      Fade fade_comp { .state { Fade::State::Idel }, .speed { 2.0f }, .black_out_time { 1.0f } };
      registry.emplace<Fade>( _fade, fade_comp );

      auto& system_manager { context_.getSystemManager() };
      system_manager.addSystem( typeid( FadeSystem ), std::make_unique<FadeSystem>( 99 ) );
   }
   void SceneManager::update( GameContext& context_ )
   {

      auto& registry { context_.getRegistry() };
      auto& fade_comp { registry.get<Fade>( _fade ) };

      if ( fade_comp.state == Fade::State::Idel )
      {
         _current_scene->update();
         if ( _next_scene ) { fade_comp.state = Fade::State::FadeOut; }
      }

      switch ( fade_comp.state )
      {
         case Fade::State::FadeOutEnd :
            _current_scene  = std::move( _next_scene );
            fade_comp.state = Fade::State::FadeIn;
            _current_scene->initialize();

            break;
         case Fade::State::FadeInEnd : _current_scene->start(); break;
      }
   }
   void SceneManager::initializeCurrentScene( std::unique_ptr<Scene> current_ )
   {
      _current_scene = std::move( current_ );
      _current_scene->initialize();
      _current_scene->start();
   }
}    // namespace sdl_engine