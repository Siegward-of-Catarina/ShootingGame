#include <cmath>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/graphics.hpp>
#include <engine/systems/sprite_render_system.hpp>
namespace sdl_engine
{
   SpriteRenderSystem::SpriteRenderSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   SpriteRenderSystem::~SpriteRenderSystem() {}

   void SpriteRenderSystem::update( GameContext& context_ )
   {
      auto& registry = context_.getRegistry();
      auto& renderer = context_.getRenderer();
      renderer.renderClear( .3f, .3f, .3f, 1.f );

      // ”Ä—p•`‰æ
      auto render_func = []( Renderer& renderer, Sprite& sprt, Transform& trfm )
      {
         SDL_SetTextureColorModFloat( sprt.texture->texture, sprt.color.r(), sprt.color.g(), sprt.color.b() );
         SDL_SetTextureAlphaModFloat( sprt.texture->texture, sprt.color.a() );

         auto harf_w = sprt.dst.w / 2;
         auto harf_h = sprt.dst.h / 2;
         sprt.dst.x  = trfm.x - harf_w;
         sprt.dst.y  = trfm.y - harf_h;
         renderer.renderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      };

      // ƒ^ƒO‚²‚Æ‚É•`‰æ
      for ( auto [ entity, sprt, trfm ] : registry.view<Sprite, Transform, RenderBackgroundTag>().each() )
      {
         render_func( renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm ] : registry.view<Sprite, Transform, RenderGameSpriteTag>().each() )
      {
         render_func( renderer, sprt, trfm );
      }

      for ( auto [ entity, sprt, trfm ] : registry.view<Sprite, Transform, RenderUITag>().each() )
      {
         render_func( renderer, sprt, trfm );
      }

      for ( auto [ entity, trfm ] : registry.view<Transform, RenderTextTag>().each() ) {}

      for ( auto [ entity, sprt, trfm ] : registry.view<Sprite, Transform, RenderFadeTag>().each() )
      {
         render_func( renderer, sprt, trfm );
      }
   }
}    // namespace sdl_engine
