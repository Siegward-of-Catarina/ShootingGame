#include <engine/basic_component.hpp>
#include <engine/core.hpp>
// event
#include <engine/events/fade_events.hpp>
#include <engine/events/fade_render_layer_change_event.hpp>
// system
#include <engine/systems/fade_system.hpp>
namespace
{
   constexpr f32 MAX_ALPHA { 1.0f };
}
namespace sdl_engine
{
   sdl_engine::FadeSystem::FadeSystem( i32 priority_, entt::registry& registry_, EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _fades {}
   {

      _event_listener.connect<&FadeSystem::onFadeOutStart, FadeOutStartEvent>( this );
      _event_listener.connect<&FadeSystem::onFadeRenderLayerChange, FadeRenderLayerChangeEvent>( this );
      _event_listener.connect<&FadeSystem::onFadeSetAlpha, FadeSetAlphaEvent>( this );
   }

   sdl_engine::FadeSystem::~FadeSystem() {}

   void sdl_engine::FadeSystem::update( const FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> erase_entts {};
      for ( auto& entity : _fades )
      {
         if ( !registry().valid( entity ) )
         {
            erase_entts.emplace_back( entity );
            continue;
         }
         auto& fade { reg.get<Fade>( entity ) };
         auto& sprt { reg.get<Sprite>( entity ) };
         switch ( fade.state )
         {
            case Fade::State::FadeIn :
               sprt.color.a -= fade.speed * frame_.delta_time;
               if ( sprt.color.a < fade.end_alpha )
               {
                  sprt.color.a = fade.end_alpha;
                  fade.state   = Fade::State::FadeInEnd;
               }
               break;
            case Fade::State::FadeOut :
               sprt.color.a += fade.speed * frame_.delta_time;
               if ( sprt.color.a > fade.target_out_alpha )
               {
                  sprt.color.a = fade.target_out_alpha;
                  fade.state   = Fade::State::BlackOut;
               }
               break;
            case Fade::State::BlackOut :
               fade.black_out_wait += frame_.delta_time;
               if ( fade.black_out_wait > fade.black_out_duration )
               {
                  fade.state          = Fade::State::FadeOutEnd;
                  fade.black_out_wait = 0.0f;
               }
               break;
            case Fade::State::FadeInEnd :
               fade.state = Fade::State::Idle;
               _event_listener.trigger<FadeInEndEvent>( { entity } );
               break;
            case Fade::State::FadeOutEnd :
               _event_listener.trigger<FadeOutEndEvent>( { entity } );
               // OutInの場合Inを始める
               if ( fade.type == Fade::Type::OutIn ) { fade.state = Fade::State::FadeIn; }
               break;
         }
      }

      for ( auto& entt : erase_entts ) { _fades.erase( entt ); }
   }
   void FadeSystem::onFadeOutStart( FadeOutStartEvent& e )
   {
      if ( registry().all_of<Fade>( e.owner ) )
      {
         _fades.emplace( e.owner );
         auto& fade { registry().get<Fade>( e.owner ) };
         auto& sprt { registry().get<Sprite>( e.owner ) };
         // start alpha override
         if ( e.start_alpha_override >= 0.0f ) { sprt.color.a = e.start_alpha_override; }
         fade.state = Fade::State::FadeOut;
         // オーバーライドが指定されていれば適用
         if ( e.end_alpha_override >= 0.0f ) { fade.end_alpha = e.end_alpha_override; }
         if ( e.start_alpha_override >= 0.0f ) { fade.target_out_alpha = e.start_alpha_override; }
         if ( e.speed_override >= 0.0f ) { fade.speed = e.speed_override; }
         if ( e.black_out_duration_override >= 0.0f ) { fade.black_out_duration = e.black_out_duration_override; }
         // target_out_alpha は end_alpha_override と意味が異なるので、別で持つ
         if ( e.target_out_alpha_override >= 0.0f ) { fade.target_out_alpha = e.target_out_alpha_override; }
      }
   }
   void FadeSystem::onFadeRenderLayerChange( FadeRenderLayerChangeEvent& e )
   {

      auto& reg { registry() };
      if ( reg.valid( e.fade_entity ) && reg.all_of<Fade>( e.fade_entity ) )
      {
         if ( e.under_ui )
         {
            if ( reg.all_of<RenderFadeTag>( e.fade_entity ) ) { reg.remove<RenderFadeTag>( e.fade_entity ); }
            reg.emplace<RenderFadeUnderUITag>( e.fade_entity );
         }
         else
         {
            if ( reg.all_of<RenderFadeUnderUITag>( e.fade_entity ) )
            {
               reg.remove<RenderFadeUnderUITag>( e.fade_entity );
            }
            reg.emplace<RenderFadeTag>( e.fade_entity );
         }
      }
   }

   void FadeSystem::onFadeSetAlpha( FadeSetAlphaEvent& e )
   {
      auto& reg { registry() };
      if ( reg.valid( e.owner ) && reg.all_of<Sprite>( e.owner ) )
      {
         auto& sprt { reg.get<Sprite>( e.owner ) };
         sprt.color.a = e.alpha;
      }
   }
}    // namespace sdl_engine