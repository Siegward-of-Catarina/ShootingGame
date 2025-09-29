#include <engine/components/sprite.hpp>
#include <engine/components/transform.hpp>
#include <engine/core/game_context.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/systems/sprite_render_system.hpp>
namespace sdl_engine
{
   SpriteRenderSystem::SpriteRenderSystem() {}
   SpriteRenderSystem::~SpriteRenderSystem() {}

   void SpriteRenderSystem::update( entt::registry& registry_, GameContext& context_, f32 delta_time_ )
   {
      auto& renderer = context_.getRenderer();

      renderer.RenderClear( .3f, .3f, .3f, 1.f );

      for ( auto [ entity, sprt ] : registry_.view<Sprite>().each() )
      {
         auto& trfm   = registry_.get<Transform>( entity );
         auto  harf_w = sprt.dst.w / 2;
         auto  harf_h = sprt.dst.h / 2;
         sprt.dst.x   = trfm.x - harf_w;
         sprt.dst.y   = trfm.y - harf_h;
         renderer.RenderTexture( sprt.texture->texture, &sprt.src, &sprt.dst, trfm.angle );
      }

      renderer.RenderPresent();
   }

   int SpriteRenderSystem::priority() const { return 100; }
}    // namespace sdl_engine
