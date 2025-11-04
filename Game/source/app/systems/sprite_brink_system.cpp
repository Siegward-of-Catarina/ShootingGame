#include <app/components/damage.hpp>
#include <app/components/sprite_brink.hpp>
#include <app/systems/sprite_brink_system.hpp>
#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

// event
#include <app/event/dead_event.hpp>
namespace myge
{
   SpriteBrinkSystem::SpriteBrinkSystem( i32                        priority_,
                                         entt::registry&            registry_,
                                         sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
   }
   SpriteBrinkSystem::~SpriteBrinkSystem() {}
   void SpriteBrinkSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> to_dead_brink;
      for ( auto [ entity, sprt, brink ] : getLogicUpdateable<sdl_engine::Sprite, SpriteBrink>( reg ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }

         if ( brink.is_cycle_end == 2 )
         {
            // 1サイクル完了 -1の場合無限ループ
            if ( brink.num_cycles > 0 ) { brink.num_cycles--; }
            brink.is_cycle_end = 0;
         }

         if ( brink.num_cycles != 0 )
         {
            switch ( brink.state )
            {
               case SpriteBrink::State::AddAlpha :
                  sprt.color.a += brink.speed * frame_.delta_time;
                  if ( sprt.color.a > 1.0f )
                  {
                     sprt.color.a = 1.0f;
                     brink.state  = SpriteBrink::State::SubAlpha;
                     brink.is_cycle_end++;
                  }
                  break;
               case SpriteBrink::State::SubAlpha :
                  sprt.color.a -= brink.speed * frame_.delta_time;
                  if ( sprt.color.a < brink.min_alpha )
                  {
                     sprt.color.a = brink.min_alpha;
                     brink.state  = SpriteBrink::State::AddAlpha;
                     brink.is_cycle_end++;
                  }
                  break;
            }
         }
         else
         {
            // num_cycles が0になったスプライトをアルファ値0にしてdeadリストへ
            sprt.color.a -= brink.speed * frame_.delta_time;
            if ( sprt.color.a < brink.min_alpha )
            {
               sprt.color.a = brink.min_alpha;
               to_dead_brink.emplace_back( entity );
            }
         }
      }

      // dead brink
      // num_cycles が0になったスプライトのdeadイベント発行
      if ( !to_dead_brink.empty() ) { _event_listener.trigger<DeadEvent>( DeadEvent { to_dead_brink } ); }

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
               sprt.color.g = 0.8f;
               sprt.color.b = 0.8f;
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
