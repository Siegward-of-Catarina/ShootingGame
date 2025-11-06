#include <pch.hpp>
// my header
#include <engine/systems/sprite_animation_system.hpp>
// component
#include <engine/components/enable_tag_components.hpp>
#include <engine/components/sprite.hpp>
#include <engine/components/sprite_anim.hpp>
// core
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
// render
#include <engine/rendering/resource/sprite_anim_resource.hpp>
// event
#include <engine/events/event_listener.hpp>
#include <engine/events/sprite_anim_end_event.hpp>
namespace
{
   constexpr size_t DEFAULT_RESERVE_SIZE { 50 };
}
namespace sdl_engine
{
   SpriteAnimationSystem::SpriteAnimationSystem( i32             priority_,
                                                 entt::registry& registry_,
                                                 EventListener&  event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _anim_end_entities {}
   {
      _anim_end_entities.reserve( DEFAULT_RESERVE_SIZE );
   }
   SpriteAnimationSystem::~SpriteAnimationSystem() {}
   void SpriteAnimationSystem::update( const FrameData& frame_ )
   {
      // 削除後リセット
      _anim_end_entities.clear();

      auto& reg { registry() };
      for ( auto [ entity, sprt, anim ] : reg.view<Sprite, SpriteAnim>().each() )
      {
         if ( !reg.valid( entity ) || !anim.is_playing || anim.sprite_anim->frame_num <= 1 ) { continue; }

         // マニュアルタイプ以外をアニメーションさせる
         if ( anim.sprite_anim->anim_type != AnimType::Manual )
         {

            // 累計時間に加算
            anim.elapsed_time += frame_.delta_time;
            // フレーム時間を超えた時点で切り替え
            if ( anim.elapsed_time >= anim.sprite_anim->frame_time )
            {
               anim.elapsed_time -= anim.sprite_anim->frame_time;
               anim.current_frame++;
               if ( anim.current_frame >= anim.sprite_anim->frame_num )
               {
                  if ( anim.sprite_anim->anim_type == AnimType::Loop ) { anim.current_frame = 0; }
                  else
                  {
                     anim.current_frame = anim.sprite_anim->frame_num - 1;
                     anim.is_playing    = false;
                     _anim_end_entities.emplace_back( entity );
                  }
               }
            }
         }
         // sprt.srcはAnimType関係なしに更新する
         sprt.src = anim.sprite_anim->frames[ anim.current_frame ];
      }

      // 前フレームの削除対象を知らせるイベントを発行する
      if ( !_anim_end_entities.empty() )
      {
         if ( !_event_listener.empty<SpriteAnimEndEvent>() )
         {
            _event_listener.trigger<SpriteAnimEndEvent>( { std::move( _anim_end_entities ) } );
         }
      }
   }
}    // namespace sdl_engine
