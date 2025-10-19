#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/events/fade_events.hpp>
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
               if ( sprt.color.a < 0.f )
               {
                  sprt.color.a = 0.0f;
                  fade.state   = Fade::State::FadeInEnd;
               }
               break;
            case Fade::State::FadeOut :
               sprt.color.a += fade.speed * frame_.delta_time;
               if ( sprt.color.a > MAX_ALPHA )
               {
                  sprt.color.a = MAX_ALPHA;
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
         fade.state = Fade::State::FadeOut;
      }
   }
}    // namespace sdl_engine