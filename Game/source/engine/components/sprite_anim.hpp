#pragma once
#include <SDL3/SDL_rect.h>
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <vector>

namespace sdl_engine
{
   // スプライトアニメーションの状態
   struct SpriteAnim
   {
      entt::resource<SpriteAnimResource> sprite_anim;
      u32                                current_frame {};
      f64                                elapsed_time {};    // 経過秒
      bool                               is_playing {};
   };

   // JSON からアニメ資源をロードして初期化
   SpriteAnim createSpriteAnim( ResourceManager& resource_manager_, const json& data_ );

   // 既存リソースから初期化
   SpriteAnim
   createSpriteAnim( entt::resource<SpriteAnimResource> resource_, u32 current_frame_ = 0, bool is_playing_ = true );
}    // namespace sdl_engine