#include <engine/Components/sprite.hpp>

namespace sdl_engine
{
   Sprite createSprite( entt::resource<sdl_engine::SpriteResource> resource_, const std::optional<u32> render_order_ )
   {
      auto               sprite { resource_ };
      sdl_engine::Sprite sprt_comp {
         .texture { sprite },
         .src { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .dst { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .color { 1.0f, 1.0f, 1.0f, 1.0f },
         .render_order { 100 }
      };
      // render orderはオプション。基本はレンダータイプごとにレイヤー分けされる。
      if ( render_order_ ) { sprt_comp.render_order = render_order_.value(); }

      return sprt_comp;
   }
}    // namespace sdl_engine