#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/systems/fade_system.hpp>
namespace
{
   constexpr f32 MAX_ALPHA { 1.0f };
}
namespace sdl_engine
{
   sdl_engine::FadeSystem::FadeSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }

   sdl_engine::FadeSystem::~FadeSystem() {}

   void sdl_engine::FadeSystem::update( sdl_engine::EngineContext& context_ )
   {
      auto& registry { getRegistry() };
      auto  delta_time { context_.getGameTimer().getDeltaTime() };

      for ( auto [ entity, fade, sprt ] : registry.view<Fade, Sprite>().each() )
      {
         switch ( fade.state )
         {
            case Fade::State::FadeIn :
               sprt.color.a() -= fade.speed * delta_time;
               if ( sprt.color.a() < 0.f )
               {
                  sprt.color.a() = 0.0f;
                  fade.state     = Fade::State::FadeInEnd;
               }
               break;
            case Fade::State::FadeOut :
               sprt.color.a() += fade.speed * delta_time;
               if ( sprt.color.a() > MAX_ALPHA )
               {
                  sprt.color.a() = MAX_ALPHA;
                  fade.state     = Fade::State::BlackOut;
               }
               break;
            case Fade::State::BlackOut :
               fade.black_out_wait += delta_time;
               if ( fade.black_out_wait > fade.black_out_duration )
               {
                  fade.state          = Fade::State::FadeOutEnd;
                  fade.black_out_wait = 0.0f;
               }
               break;
            case Fade::State::FadeInEnd : fade.state = Fade::State::Idel; break;
            case Fade::State::FadeOutEnd :
               // OutInの場合Inを始める
               if ( fade.type == Fade::Type::OutIn ) { fade.state = Fade::State::FadeIn; }
               break;
         }
      }
   }
}    // namespace sdl_engine