#include <engine/Components/enable_tag_components.hpp>
#include <engine/components/sprite.hpp>
#include <engine/components/sprite_anim.hpp>
#include <engine/core/game_context.hpp>
#include <engine/core/game_timer.hpp>
#include <engine/rendering/resource/sprite_anim_resource.hpp>
#include <engine/systems/sprite_animation_system.hpp>
namespace sdl_engine
{
   SpriteAnimationSystem::SpriteAnimationSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   SpriteAnimationSystem::~SpriteAnimationSystem() {}
   void SpriteAnimationSystem::update( GameContext& context_ )
   {
      auto& registry   = context_.getRegistry();
      auto  delta_time = context_.getGameTimer().getDeltaTime();
      for ( auto [ entity, sprt, anim ] : registry.view<Sprite, SpriteAnim>().each() )
      {
         if ( !anim.is_playing || anim.sprite_anim->frame_num <= 1 ) { continue; }

         // マニュアルタイプ以外をアニメーションさせる
         if ( anim.sprite_anim->anim_type != AnimType::Manual )
         {

            // 累計時間に加算
            anim.elapsed_time += delta_time;
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
                  }
               }
            }
         }
         // sprt.srcはAnimType関係なしに更新する
         sprt.src = anim.sprite_anim->frames[ anim.current_frame ];
      }
   }
}    // namespace sdl_engine
