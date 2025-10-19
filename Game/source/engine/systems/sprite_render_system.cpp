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
      _renderer.renderClear( .3f, .3f, .3f, 1.f );

      // 汎用描画
      auto common_render = []( Renderer& renderer, Sprite& sprt, Transform& trfm )
      {
         SDL_SetTextureColorModFloat( sprt.texture->texture, sprt.color.r, sprt.color.g, sprt.color.b );
         SDL_SetTextureAlphaModFloat( sprt.texture->texture, sprt.color.a );

         auto harf_w = sprt.dst.w / 2;
         auto harf_h = sprt.dst.h / 2;
         sprt.dst.x  = trfm.x - harf_w;
         sprt.dst.y  = trfm.y - harf_h;
         renderer.renderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      };

      // タグごとに描画
      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderBackgroundTag>().each() )
      {
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderGameSpriteTag>().each() )
      {
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderUITag>().each() )
      {
         common_render( _renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm, text ] : reg.view<Sprite, Transform, Text, RenderTextTag>().each() )
      {
         RenderTransform r_trfm { trfm.x, trfm.y, trfm.angle, trfm.scale };
         _renderer.renderText( sprt.texture->texture, *text.font, text.text, r_trfm, sprt.color );
      }

      for ( auto [ entity, sprt, trfm ] : reg.view<Sprite, Transform, RenderFadeTag>().each() )
      {
         common_render( _renderer, sprt, trfm );
      }
   }
}    // namespace sdl_engine
