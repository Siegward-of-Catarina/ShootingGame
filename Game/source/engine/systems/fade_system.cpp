#include <pch.hpp>
// my header
#include <engine/systems/fade_system.hpp>
// component
#include <engine/basic_component.hpp>
// core
#include <engine/core.hpp>
// event
#include <engine/events/fade_events.hpp>
#include <engine/events/fade_render_layer_change_event.hpp>

namespace
{
   constexpr f32 MAX_ALPHA { 1.0f };
}

namespace sdl_engine
{
   // フェードイベントを購読して更新対象を管理
   sdl_engine::FadeSystem::FadeSystem( i32 priority_, entt::registry& registry_, EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _fades {}
   {
      _fades.reserve( 32 );
      _event_listener.connect<&FadeSystem::onFadeOutStart, FadeOutStartEvent>( this );
      _event_listener.connect<&FadeSystem::onFadeRenderLayerChange, FadeRenderLayerChangeEvent>( this );
      _event_listener.connect<&FadeSystem::onFadeSetAlpha, FadeSetAlphaEvent>( this );
   }

   sdl_engine::FadeSystem::~FadeSystem() {}

   void sdl_engine::FadeSystem::update( const FrameData& frame_ )
   {
      auto& reg { registry() };
      // 更新対象のフェードを収集（RenderFadeTag/RenderFadeUnderUITag を持つ）
      for ( auto [ entity, comp ] : reg.view<Fade>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         _fades.emplace( entity );
      }

      for ( auto entity : _fades )
      {
         if ( !reg.valid( entity ) || !reg.all_of<Fade>( entity ) ) { continue; }
         auto& fade { reg.get<Fade>( entity ) };
         auto& sprite { reg.get<Sprite>( entity ) };
         switch ( fade.state )
         {
            case Fade::State::FadeIn :
               sprite.color.a -= fade.speed * frame_.delta_time;
               if ( sprite.color.a < 0.001f )
               {
                  sprite.color.a = 0.0f;
                  fade.state     = Fade::State::Idle;
               }
               break;

            case Fade::State::FadeOut :
               sprite.color.a += fade.speed * frame_.delta_time;
               if ( sprite.color.a > fade.target_out_alpha - 0.001f )
               {
                  sprite.color.a = fade.target_out_alpha;
                  fade.state     = Fade::State::BlackOut;    // 次はブラックアウト
               }
               break;

            case Fade::State::BlackOut :
               fade.black_out_duration -= frame_.delta_time;
               if ( fade.black_out_duration <= 0.0f ) { fade.state = Fade::State::FadeIn; }
               break;

            case Fade::State::Idle :
            default : break;
         }
      }
      // 更新対象集合をクリア
      _fades.clear();
   }

   void FadeSystem::onFadeOutStart( FadeOutStartEvent& e )
   {
      auto& reg { registry() };
      if ( reg.valid( e.owner ) && reg.all_of<Fade>( e.owner ) )
      {
         auto& fade { reg.get<Fade>( e.owner ) };
         fade.state = Fade::State::FadeOut;
      }
   }

   void FadeSystem::onFadeRenderLayerChange( FadeRenderLayerChangeEvent& e )
   {
      auto& reg { registry() };
      if ( reg.valid( e.fade_entity ) )
      {
         // UI 下/上のタグを切り替え
         if ( e.under_ui )
         {
            reg.emplace<RenderFadeUnderUITag>( e.fade_entity );
            reg.remove<RenderFadeTag>( e.fade_entity );
         }
         else
         {
            reg.emplace<RenderFadeTag>( e.fade_entity );
            reg.remove<RenderFadeUnderUITag>( e.fade_entity );
         }
      }
   }

   void FadeSystem::onFadeSetAlpha( FadeSetAlphaEvent& e )
   {
      auto& reg { registry() };
      if ( reg.valid( e.owner ) && reg.all_of<Sprite>( e.owner ) )
      {
         auto& sprite { reg.get<Sprite>( e.owner ) };
         sprite.color.a = e.alpha;
      }
   }
}    // namespace sdl_engine