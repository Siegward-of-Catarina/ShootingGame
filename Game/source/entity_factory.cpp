#include "entity_factory.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <components/sprite_anim_component.hpp>
#include <components/transform_component.hpp>
#include <components/velocity_component.hpp>
#include <resource_manager/resource_manager.hpp>
entt::entity
myge::EntityFactory::createEnemy( entt::registry& registry_, ResourceManager& resource_manager_, float px_, float py_ )
{
   auto enemy = registry_.create();

   auto image = resource_manager_.getTexture( "enemy" );

   registry_.emplace<TransFormComponent>( enemy, .0f, .0f );
   registry_.emplace<VelocityComponent>( enemy, 0.0f, 100.0f );

   auto& sprite = registry_.emplace<SpriteAnimComponent>( enemy, image, 8, 1 );

   auto col = sprite.w;
   auto row = sprite.h;
   auto size { col * row };
   sprite.sprites.reserve( size );
   sprite.sprites.resize( size );

   sprite.w = image->w / sprite.w;
   sprite.h = image->h / sprite.h;

   for ( int i = 0; i < size; i++ )
   {
      sprite.sprites[ i ].x = sprite.w * ( i % col );
      sprite.sprites[ i ].y = sprite.h * ( i / col );

      sprite.sprites[ i ].w = sprite.w;
      sprite.sprites[ i ].h = sprite.h;
   }

   return enemy;
}
