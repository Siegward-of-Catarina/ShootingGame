#include <SDL3/SDL.h>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/rendering/resource/sprite_anim_resource.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <engine/utils/json_utilities.hpp>
#include <nlohmann/json.hpp>

namespace
{

   u32 u32Hash( std::string_view key ) { return static_cast<u32>( std::hash<std::string_view>()( key.data() ) ); }
}    // namespace

namespace sdl_engine
{
   ResourceManager::ResourceManager() {}
   ResourceManager::~ResourceManager() {}

   void ResourceManager::loadResources( Renderer& renderer_, std::string_view assets_path_ )
   {
      // assets読み込み
      auto assets = loadJson( assets_path_ );
      // sprite読み込み
      if ( assets.contains( "Sprites" ) )
      {
         auto sprites { assets.at( "Sprites" ) };

         // ルートパスを取得。末尾が「 / 」じゃない場合追加する
         auto sprite_root_path { getJsonData<std::string>( sprites, "SpriteRootPath" ).value() };
         if ( !sprite_root_path.ends_with( '/' ) ) { sprite_root_path += '/'; }

         auto spritedatas { sprites.at( "SpriteDatas" ) };
         for ( auto& sprt : spritedatas )
         {
            // pathを連結させる
            sprt[ "file_path" ] = sprite_root_path + sprt.at( "file_path" ).get<std::string>();

            // nameからhashを生成して登録
            auto name { getJsonData<std::string>( sprt, "name" ).value() };
            auto handle { _sprite_cache.load( u32Hash( name ), renderer_, sprt ) };
         }
      }

      // spriteアニメーション読み込み
      if ( assets.contains( "SpriteAnims" ) )
      {
         auto sprite_anims { assets.at( "SpriteAnims" ) };
         for ( const auto& anim : sprite_anims )
         {
            // nameからhashを生成して登録
            auto name { getJsonData<std::string>( anim, "name" ).value() };

            auto handle { _sprite_anim_cache.load( u32Hash( name ), anim ) };
         }
      }

      // fontデータ読み込み
      if ( assets.contains( "FontDatas" ) )
      {
         auto font_datas { assets.at( "FontDatas" ) };
         for ( const auto& data : font_datas )
         {
            // nameからhashを生成して登録
            auto name { getJsonData<std::string>( data, "name" ).value() };
            auto handle { _font_cache.load( u32Hash( name ), data ) };
         }
      }
   }

   void ResourceManager::addSpriteResources( std::string_view resources_name_, SDL_Texture* texture_ )
   {
      auto handle = _sprite_cache.load( u32Hash( resources_name_.data() ), texture_ );
   }

   entt::resource<SpriteResource> ResourceManager::getSprite( std::string_view key_ )
   {
      if ( entt::resource<SpriteResource> res = _sprite_cache[ u32Hash( key_ ) ]; res ) { return res; }
      return {};
   }
   entt::resource<SpriteAnimResource> ResourceManager::getSpriteAnim( std::string_view key_ )
   {
      if ( entt::resource<SpriteAnimResource> res = _sprite_anim_cache[ u32Hash( key_ ) ]; res ) { return res; }
      return {};
   }
   entt::resource<FontResource> ResourceManager::getFont( std::string_view key_ )
   {
      if ( entt::resource<FontResource> res = _font_cache[ u32Hash( key_ ) ]; res ) { return res; }
      return {};
   }
}    // namespace sdl_engine