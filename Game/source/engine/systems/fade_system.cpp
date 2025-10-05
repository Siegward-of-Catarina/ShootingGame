#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/systems/fade_system.hpp>
namespace
{
   f32 black_out_timer { 0.0f };
}
namespace sdl_engine
{
   sdl_engine::FadeSystem::FadeSystem( i32 priority_ ) : SystemInterface( priority_ ) {}

   sdl_engine::FadeSystem::~FadeSystem() {}

   void sdl_engine::FadeSystem::update( sdl_engine::GameContext& context_ )
   {
      auto& registry { context_.getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };

      for ( auto [ entity, fade, sprt ] : registry.view<Fade, Sprite>().each() )
      {
         switch ( fade.state )
         {
            case Fade::State::FadeOut :
               sprt.color.a += fade.speed * delta_time;
               if ( sprt.color.a > 1.f )
               {
                  sprt.color.a = 1.0f;
                  fade.state   = Fade::State::BlackOut;
               }
               break;
            case Fade::State::BlackOut :
               black_out_timer += delta_time;
               if ( black_out_timer > fade.black_out_time )
               {
                  fade.state      = Fade::State::FadeOutEnd;
                  black_out_timer = 0.0f;
               }
               break;
            case Fade::State::FadeIn :
               sprt.color.a -= fade.speed * delta_time;
               if ( sprt.color.a < 0.f )
               {
                  sprt.color.a = 0.0f;
                  fade.state   = Fade::State::FadeInEnd;
               }
               break;
            case Fade::State::FadeInEnd : fade.state = Fade::State::Idel; break;
         }
      }
   }
}    // namespace sdl_engine