#pragma once
#include <core/fwd.hpp>
#include <entt/entt.hpp>
#include <resource/loader/sprite_loader.hpp>
#include <resource/loader/sprite_anim_loader.hpp>
namespace myge
{
   using TextureCache    = entt::resource_cache<SDL_Texture, SpriteLoader>;
   using SpriteAnimCache = entt::resource_cache<SpriteAnimResource, SpriteAnimLoader>;

   class ResourceManager
   {
   public:
      ResourceManager();
      ~ResourceManager();
      void loadResources( Renderer& renderer_, std::string_view assets_path_ );

      // 今回は規模的に個々のデータごとにgetterを用意する
      entt::resource<SDL_Texture> getSprite( std::string_view key );
      entt::resource<SpriteAnimResource> getSpriteAnim(std::string_view key);
   private:
      TextureCache    _sprite_cache;
      SpriteAnimCache _sprite_anim_cache;
   };
}    // namespace myge