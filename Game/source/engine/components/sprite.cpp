#include <engine/Components/sprite.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
namespace sdl_engine
{
   Sprite createSprite( ResourceManager& resource_manager_, const json& data_ )
   {
      auto sprite_name { sdl_engine::getJsonData<std::string>( data_, "name" ) };
      auto sprite = resource_manager_.getSprite( sprite_name.value() );

      sdl_engine::Sprite sprt_comp {
         .texture { sprite },
         .src { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .dst { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .color { 1.0f, 1.0f, 1.0f, 1.0f },
         .render_order { 100 }
      };
      // color
      if ( auto color { getJsonData<std::array<f32, 4>>( data_, "color" ) }; color )
      {
         sprt_comp.color.data = color.value();
      }
      // render orderはオプション。基本はレンダータイプごとにレイヤー分けされる。
      if ( auto order { getJsonData<u32>( data_, "render_order" ) }; order ) { sprt_comp.render_order = order.value(); }

      return sprt_comp;
   }
   Sprite createSprite( entt::resource<SpriteResource> sprite_resource_, ColorRGBA color_, u32 render_order_ )
   {
      auto sprite = sprite_resource_;

      sdl_engine::Sprite sprt_comp {
         .texture { sprite },
         .src { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .dst { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .color { color_ },
         .render_order { render_order_ }
      };

      return sprt_comp;
   }
}    // namespace sdl_engine