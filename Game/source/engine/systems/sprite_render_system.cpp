#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/systems/sprite_render_system.hpp>
//
#include <engine/rendering/resource/font_resource.hpp>
namespace sdl_engine
{
   SpriteRenderSystem::SpriteRenderSystem( i32 priority_, entt::registry& registry_, Renderer& renderer_ )
     : SystemInterface { priority_, registry_ }, _renderer { renderer_ }
   {
   }
   SpriteRenderSystem::~SpriteRenderSystem() {}

   void SpriteRenderSystem::update( [[maybe_unused]] const FrameData& frame_ )
   {
      auto& reg { registry() };
      _renderer.renderClear( .0f, .0f, .0f, 1.f );

      // 汎用描画
      auto common_render = []( Renderer& renderer, Sprite& sprt, Transform& trfm )
      {
         SDL_SetTextureColorModFloat( sprt.texture->texture, sprt.color.r, sprt.color.g, sprt.color.b );
         SDL_SetTextureAlphaModFloat( sprt.texture->texture, sprt.color.a );

         auto harf_w = sprt.dst.w / 2;
         auto harf_h = sprt.dst.h / 2;
         sprt.dst.x  = trfm.position.x - harf_w;
         sprt.dst.y  = trfm.position.y - harf_h;
         renderer.renderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      };

      // タグごとに描画
      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderBackgroundTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderGameSpriteTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         common_render( _renderer, sprt, trfm );
      }
      // UIの下にフェードを描画したい場合このタグで描画を切り替えることができるs
      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderFadeUnderUITag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, repeat, trfm ] : reg.view<Sprite, RepeatSprite, Transform, RenderUITag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         for ( i32 i = 0; i < repeat.count; ++i )
         {
            f32 offset_x = repeat.offset.x * i;
            f32 offset_y = repeat.offset.y * i;
            // Gridタイプの場合は列数に応じてオフセットを調整
            if ( repeat.type == RepeatSprite::Type::Grid )
            {
               i32 row    = i / repeat.columns;
               i32 column = i % repeat.columns;
               offset_x   = repeat.offset.x * column + repeat.offset_column.x * row;
               offset_y   = repeat.offset.y * row + repeat.offset_column.y * row;
            }
            // Transformをコピーしてオフセットを適用
            Transform trfm_modified = trfm;
            trfm_modified.position.x += offset_x;
            trfm_modified.position.y += offset_y;

            common_render( _renderer, sprt, trfm_modified );
         }
      }
      for ( auto [ entity, sprt, trfm ] :
            reg.view<Sprite, Transform, RenderUITag>( entt::exclude<RepeatSprite> ).each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm, text ] : reg.view<Sprite, Transform, Text, RenderTextTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         RenderTransform r_trfm { trfm.position.x, trfm.position.y, trfm.angle, trfm.scale };
         _renderer.renderText( sprt.texture->texture, *text.font, text.text, r_trfm, sprt.color );
      }

      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderFadeTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         common_render( _renderer, sprt, trfm );
      }
   }
}    // namespace sdl_engine
