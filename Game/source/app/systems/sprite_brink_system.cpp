#include <app/components/damage.hpp>
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
   void SpriteBrinkSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, sprt, brink ] : getLogicUpdateable<sdl_engine::Sprite, SpriteBrink>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         switch ( brink.state )
         {
            case SpriteBrink::State::AddAlpha :
               sprt.color.a += brink.speed * frame_.delta_time;
               if ( sprt.color.a > 1.0f )
               {
                  sprt.color.a = 1.0f;
                  brink.state  = SpriteBrink::State::SubAlpha;
               }
               break;
            case SpriteBrink::State::SubAlpha :
               sprt.color.a -= brink.speed * frame_.delta_time;
               if ( sprt.color.a < brink.min_alpha )
               {
                  sprt.color.a = brink.min_alpha;
                  brink.state  = SpriteBrink::State::AddAlpha;
               }
               break;
         }
      }

      std::vector<entt::entity> to_remove;
      for ( auto [ entity, sprt, damage ] : getLogicUpdateable<sdl_engine::Sprite, DamageEffect>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         damage.elapsed_time += frame_.delta_time;
         if ( damage.elapsed_time >= damage.red_brink_time )
         {
            sprt.color.r        = 1.0f;
            sprt.color.g        = 1.0f;
            sprt.color.b        = 1.0f;
            damage.elapsed_time = 0.0f;
            to_remove.emplace_back( entity );
         }
         else
         {
            f32 cycle = std::fmodf( damage.elapsed_time, damage.brink_interval * 2.0f );
            if ( cycle < damage.brink_interval )
            {
               sprt.color.g = 1.0f;
               sprt.color.b = 1.0f;
            }
            else
            {
               sprt.color.g = 0.0f;
               sprt.color.b = 0.0f;
            }
         }
      }

      for ( auto entity : to_remove ) { registry().remove<DamageEffect>( entity ); }
   }
}    // namespace myge
