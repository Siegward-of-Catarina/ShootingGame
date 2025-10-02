#include <engine/components/sprite.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/systems/sprite_render_system.hpp>
namespace sdl_engine
{
   SpriteRenderSystem::SpriteRenderSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   SpriteRenderSystem::~SpriteRenderSystem() {}

   void SpriteRenderSystem::update( GameContext& context_ )
   {
      auto& registry = context_.getRegistry();
      auto& renderer = context_.getRenderer();
      renderer.RenderClear( .3f, .3f, .3f, 1.f );

      // 描画用にエンティティをソート
      context_.getRegistry().sort<sdl_engine::Sprite>(
        []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
        { return l_sprt.texture->depth > r_sprt.texture->depth; } );

      for ( auto [ entity, sprt ] : registry.view<Sprite>().each() )
      {
         auto& trfm   = registry.get<Transform>( entity );
         auto  harf_w = sprt.dst.w / 2;
         auto  harf_h = sprt.dst.h / 2;
         sprt.dst.x   = trfm.x - harf_w;
         sprt.dst.y   = trfm.y - harf_h;
         renderer.RenderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      }

      renderer.RenderPresent();
   }
}    // namespace sdl_engine
