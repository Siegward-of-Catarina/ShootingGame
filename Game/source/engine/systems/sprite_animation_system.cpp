#include <engine/components/sprite.hpp>
#include <engine/components/sprite_anim.hpp>
#include <engine/rendering/loader/sprite_anim_resource.hpp>
#include <engine/systems/sprite_animation_system.hpp>
namespace sdl_engine
{
   SpriteAnimationSystem::SpriteAnimationSystem() {}
   SpriteAnimationSystem::~SpriteAnimationSystem() {}
   void
   SpriteAnimationSystem::update( entt::registry& registry_, [[maybe_unused]] GameContext& context_, f32 delta_time_ )
   {
      for ( auto [ entity, sprt, anim ] : registry_.view<Sprite, SpriteAnim>().each() )
      {
         if ( !anim.is_playing || anim.sprite_anim->frame_num <= 1 ) { continue; }

         anim.elapsed_time += delta_time_;

         if ( anim.elapsed_time >= anim.sprite_anim->frame_time )
         {
            anim.elapsed_time -= anim.sprite_anim->frame_time;
            anim.current_frame++;
            if ( anim.current_frame >= anim.sprite_anim->frame_num )
            {
               if ( anim.sprite_anim->is_loop ) { anim.current_frame = 0; }
               else
               {
                  anim.current_frame = anim.sprite_anim->frame_num - 1;
                  anim.is_playing    = false;
               }
            }
         }

         sprt.src = anim.sprite_anim->frames[ anim.current_frame ];
      }
   }

   int SpriteAnimationSystem::priority() const { return 99; }
}    // namespace sdl_engine
