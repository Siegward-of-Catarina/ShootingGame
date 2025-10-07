#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/loader/sprite_anim_loader.hpp>
#include <engine/rendering/resource/loader/sprite_loader.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   using TextureCache    = entt::resource_cache<SpriteResource, SpriteLoader>;
   using SpriteAnimCache = entt::resource_cache<SpriteAnimResource, SpriteAnimLoader>;

   class ResourceManager
   {
   public:
      ResourceManager();
      ~ResourceManager();
      void loadResources( Renderer& renderer_, std::string_view assets_path_ );
      void addSpriteResources( std::string_view resources_name_, SDL_Texture* texture_, i32 depth_ );
      // 今回は規模的に個々のデータごとにgetterを用意する
      entt::resource<SpriteResource>     getSprite( std::string_view key );
      entt::resource<SpriteAnimResource> getSpriteAnim( std::string_view key );

   private:
      TextureCache    _sprite_cache;
      SpriteAnimCache _sprite_anim_cache;
   };
}    // namespace sdl_engine