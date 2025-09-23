#include <Components/sprite_anim_component.hpp>
#include <Components/transform_component.hpp>
#include <SDL3/SDL.h>
#include <entity_factory.hpp>
#include <game.hpp>
#include <renderer/renderer.hpp>
#include <resource_manager/resource_manager.hpp>
#include <test_scene.hpp>

namespace
{
   float    frame { 0 };
   uint32_t max_frame { 8 };
   float    x { 200 };
   float    y { 800 };

}    // namespace

namespace myge
{
   TestScene::TestScene( SceneInitDesc desc_ ) : Scene { desc_ }, enemy { 0 }
   {
      resourceManager().loadResources( renderer(), "resource/image/enemy01_sprite.png" );
      EntityFactory factory;
      enemy = factory.createEnemy( registry, resourceManager(), 0, 0 );
   }
   TestScene::~TestScene() {}
   void myge::TestScene::proc( float _delta )
   {
      renderer().setRenderClearColor( .3f, .3f, .3f, 1.f );

      renderer().RenderClear();
      for ( auto [ entity, transform, sprite_anim ] : registry.view<TransFormComponent, SpriteAnimComponent>().each() )
      {
         frame += _delta * 30.0f;
         auto idx = static_cast<u32>( frame ) % max_frame;
         SDL_Log( "idx:%d", idx );
         SDL_FRect drect = { transform.x, transform.y, sprite_anim.w, sprite_anim.h };
         renderer().RenderTexture( sprite_anim.texture.get(), &sprite_anim.sprites[ idx ], &drect );
      }

      renderer().RenderPresent();
   }
}    // namespace myge
