#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/rendering/resource/loader/font_loader.hpp>
#include <engine/rendering/resource/loader/sprite_anim_loader.hpp>
#include <engine/rendering/resource/loader/sprite_loader.hpp>
#include <engine/sound/resource/loader/sound_loader.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   using TextureCache    = entt::resource_cache<SpriteResource, SpriteLoader>;
   using SpriteAnimCache = entt::resource_cache<SpriteAnimResource, SpriteAnimLoader>;
   using FontCache       = entt::resource_cache<FontResource, FontLoader>;
   using SoundCache      = entt::resource_cache<SoundResource, SoundLoader>;

   class ResourceManager
   {
   public:
      ResourceManager();
      ~ResourceManager();
      void loadResources( Renderer& renderer_, SoundMixer& soundmixer_, std::string_view assets_path_ );
      void addSpriteResources( std::string_view resources_name_, SDL_Texture*& texture_ );
      // 今回は規模的に個々のデータごとにgetterを用意する
      entt::resource<SpriteResource>     getSprite( std::string_view key_ );
      entt::resource<SpriteAnimResource> getSpriteAnim( std::string_view key_ );
      entt::resource<FontResource>       getFont( std::string_view key_ );
      entt::resource<SoundResource>      getSound( std::string_view key_ );

   private:
      TextureCache    _sprite_cache;
      SpriteAnimCache _sprite_anim_cache;
      FontCache       _font_cache;
      SoundCache      _sound_cache;
   };
}    // namespace sdl_engine