#include <pch.hpp>
// my header
#include <engine/managers/resource_manager.hpp>
// render
#include <engine/rendering/renderer.hpp>
// resource
#include <engine/rendering/resource/sprite_anim_resource.hpp>
#include <engine/rendering/resource/sprite_resource.hpp>
#include <engine/sound/resource/sound_resource.hpp>
// utisl
#include <engine/utils/json_utilities.hpp>

namespace
{
   // nameを安定してハッシュ化（string_view全体を対象にする。data()の生ポインタは使わない）
   u32 u32Hash( std::string_view key ) { return static_cast<u32>( std::hash<std::string_view> {}( key ) ); }
}    // namespace

namespace sdl_engine
{
   ResourceManager::ResourceManager() {}
   ResourceManager::~ResourceManager() {}

   void ResourceManager::loadResources( Renderer& renderer_, SoundMixer& soundmixer_, std::string_view assets_path_ )
   {
      // assets読み込み
      auto assets = loadJson( assets_path_ );

      // Sprites 読み込み
      if ( auto sprites { tryGetJson( assets, "Sprites" ) }; sprites )
      {
         // ルートパスを取得。末尾が「/」じゃない場合は追加
         auto sprite_root_path { getRequireData<std::string>( sprites->get(), "SpriteRootPath" ) };
         if ( !sprite_root_path.ends_with( '/' ) ) { sprite_root_path += '/'; }

         // 元JSONは破壊しない: ローダーに渡す最小JSONだけを生成して渡す
         if ( auto spritedatas { tryGetJson( sprites->get(), "SpriteDatas" ) }; spritedatas )
         {
            for ( const auto& data : spritedatas->get() )
            {

               // 相対パス + ルートの連結
               auto rel_path = getRequireData<std::string>( data, "file_path" );

               // SpriteLoaderが必要とする最小データのみ構築（file_pathのみ）
               json loader_input           = json::object();
               loader_input[ "file_path" ] = sprite_root_path + rel_path;

               // nameからhashを生成して登録
               auto name   = getRequireData<std::string>( data, "name" );
               auto handle = _sprite_cache.load( u32Hash( name ), renderer_, loader_input );
            }
         }
      }

      // SpriteAnims 読み込み
      if ( auto sprite_anims { tryGetJson( assets, "SpriteAnims" ) }; sprite_anims )
      {
         for ( const auto& data : sprite_anims->get() )
         {
            // nameからhashを生成して登録
            auto name   = getRequireData<std::string>( data, "name" );
            auto handle = _sprite_anim_cache.load( u32Hash( name ), data );
         }
      }

      // SoundDatas 読み込み
      if ( auto sounds { tryGetJson( assets, "Sounds" ) }; sounds )
      {
         // ルートパスを取得。末尾が「/」じゃない場合は追加
         auto sound_root_path { getRequireData<std::string>( sounds->get(), "SoundRootPath" ) };
         if ( !sound_root_path.ends_with( '/' ) ) { sound_root_path += '/'; }
         if ( auto sound_datas { tryGetJson( sounds->get(), "SoundDatas" ) }; sound_datas )
         {
            for ( const auto& data : sound_datas->get() )
            {
               // 相対パス + ルートの連結
               auto rel_path = getRequireData<std::string>( data, "file_path" );
               // SoundLoaderが必要とする最小データのみ構築（file_pathのみ）
               json loader_input           = json::object();
               loader_input[ "file_path" ] = sound_root_path + rel_path;
               // nameからhashを生成して登録
               auto name   = getRequireData<std::string>( data, "name" );
               auto handle = _sound_cache.load( u32Hash( name ), soundmixer_, loader_input );
            }
         }
      }

      // FontDatas 読み込み（tryGetJsonで一貫）
      if ( auto font_datas { tryGetJson( assets, "FontDatas" ) }; font_datas )
      {
         for ( const auto& data : font_datas->get() )
         {
            // nameからhashを生成して登録
            auto name   = getRequireData<std::string>( data, "name" );
            auto handle = _font_cache.load( u32Hash( name ), data );
         }
      }
   }

   void ResourceManager::addSpriteResources( std::string_view resources_name_, SDL_Texture*& texture_ )
   {
      // 既存テクスチャをそのままキャッシュに追加するユースケース
      auto handle = _sprite_cache.load( u32Hash( resources_name_ ), texture_ );
      (void)handle;
   }

   // 取得系は見つからなければ空のentt::resourceを返す
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
   entt::resource<SoundResource> ResourceManager::getSound( std::string_view key_ )
   {
      if ( entt::resource<SoundResource> res = _sound_cache[ u32Hash( key_ ) ]; res ) { return res; }
      return {};
   }
}    // namespace sdl_engine