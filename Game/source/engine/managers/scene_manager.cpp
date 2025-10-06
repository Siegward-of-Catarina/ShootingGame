#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/scene/scene.hpp>
#include <engine/systems/fade_system.hpp>
namespace
{
   entt::entity createFade( sdl_engine::GameContext& context_ )
   {

      auto& registry { context_.getRegistry() };
      auto& window_size { context_.getWindowSize() };
      auto  fade { registry.create() };
      // transfrom
      sdl_engine::Transform trfm_cmp {
         .x { window_size.x / 2.0f }, .y { window_size.y / 2.0f }, .angle { 0.0f }, .scale { 1.0f }
      };
      registry.emplace<sdl_engine::Transform>( fade, trfm_cmp );
      // sprite
      sdl_engine::Sprite sprt_comp {};
      sprt_comp.texture = context_.getResourceManager().getSprite( "white" );
      sprt_comp.src     = { 0.0f, 0.0f, static_cast<f32>( window_size.x ), static_cast<f32>( window_size.y ) };
      sprt_comp.dst = { trfm_cmp.x, trfm_cmp.y, static_cast<f32>( window_size.x ), static_cast<f32>( window_size.y ) };
      sprt_comp.color = { 0.0f, 0.0f, 0.0f, 0.0f };
      registry.emplace<sdl_engine::Sprite>( fade, sprt_comp );
      // fade
      sdl_engine::Fade fade_comp { .state { sdl_engine::Fade::State::Idel },
                                   .speed { 2.0f },
                                   .black_out_time { 1.0f } };
      registry.emplace<sdl_engine::Fade>( fade, fade_comp );

      return fade;
   }
}    // namespace
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

      _fade = createFade( context_ );

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
            // シーン入れ替え&初期化後フェードインへ
            _current_scene = std::move( _next_scene );
            _current_scene->initialize();
            fade_comp.state = Fade::State::FadeIn;
            break;
         case Fade::State::FadeInEnd :
            // フェードインが明けたらシーン開始
            _current_scene->start();
            break;
      }
   }
   void SceneManager::initStartCurrentScene( std::unique_ptr<Scene> current_ )
   {
      _current_scene = std::move( current_ );
      _current_scene->initialize();
      _current_scene->start();
   }
}    // namespace sdl_engine