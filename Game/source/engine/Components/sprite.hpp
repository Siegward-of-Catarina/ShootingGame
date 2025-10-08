#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/color.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   struct Sprite
   {
      entt::resource<SpriteResource> texture;
      SDL_FRect                      src;
      SDL_FRect                      dst;
      ColorRGBA                      color;
      // render orderはオプション。初期値100　基本はレンダータイプごとにレイヤー分けされる。
      // そのレイヤー内で優先順位をつけたい場合設定
      u32 render_order;
   };

   Sprite createSprite( entt::resource<sdl_engine::SpriteResource> resource_, const std::optional<u32> render_order_ );
}    // namespace sdl_engine