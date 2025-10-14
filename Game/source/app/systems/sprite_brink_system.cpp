#include <app/components/sprite_brink.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace myge
{
   SpriteBrinkSystem::SpriteBrinkSystem( i32 priority_, entt::registry& registry_ )
     : SystemInterface { priority_, registry_ }
   {
   }
   SpriteBrinkSystem::~SpriteBrinkSystem() {}
   void SpriteBrinkSystem::update(const sdl_engine::FrameData& frame_)
   {

      for ( auto [ entity, sprt, brink ] : getLogicUpdateable<sdl_engine::Sprite, SpriteBrink>( registry() ).each() )
      {

         switch ( brink.state )
         {
            case SpriteBrink::State::AddAlpha :
               sprt.color.a() += brink.speed * frame_.delta_time;
               if ( sprt.color.a() > 1.0f )
               {
                  sprt.color.a() = 1.0f;
                  brink.state    = SpriteBrink::State::SubAlpha;
               }
               break;
            case SpriteBrink::State::SubAlpha :
               sprt.color.a() -= brink.speed * frame_.delta_time;
               if ( sprt.color.a() < brink.min_alpha )
               {
                  sprt.color.a() = brink.min_alpha;
                  brink.state    = SpriteBrink::State::AddAlpha;
               }
               break;
         }
      }
   }
}    // namespace myge
