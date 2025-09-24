#include <SDL3/SDL.h>
#include <core/utilities.hpp>
#include <functional>
#include <nlohmann/json.hpp>
#include <renderer/renderer.hpp>
#include <resource/resource_manager.hpp>
#include <resource/sprite_anim_resource.hpp>

namespace
{

   u32 u32Hash( std::string_view key ) { return static_cast<u32>( std::hash<std::string_view>()( key.data() ) ); }
}    // namespace

namespace myge
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
         auto sprites = assets[ "Sprites" ];

         // ルートパスを取得。末尾が「 / 」じゃない場合追加する
         auto sprite_root_path = sprites[ "SpriteRootPath" ].get<std::string>();
         if ( !sprite_root_path.ends_with( '/' ) ) { sprite_root_path += '/'; }

         auto spritedatas = sprites[ "SpriteDatas" ];
         for ( const auto& sprt : spritedatas )
         {
            auto sprite_path = sprite_root_path + sprt[ "file" ].get<std::string>();
            if ( !std::filesystem::exists( sprite_path ) )
            {
               // runtime error に変更予定
               SDL_Log( "画像ファイルが存在しません:  %s ", sprite_path.c_str() );
               continue;
            }

            // nameからhashを生成して登録
            auto name   = sprt[ "name" ].get<std::string>();
            auto handle = _sprite_cache.load( u32Hash( name ), renderer_, sprite_path );
         }
      }

      // spriteアニメーション読み込み
      if ( assets.contains( "SpriteAnims" ) )
      {
         auto sprite_anims = assets[ "SpriteAnims" ];
         for ( const auto& anim : sprite_anims )
         {

            // nameからhashを生成して登録
            auto name        = anim[ "name" ].get<std::string>();
            auto sprite_name = anim[ "sprite_name" ].get<std::string>();
            // アニメーションと紐づけるスプライトが存在するか確認
            if ( !_sprite_cache[ u32Hash( sprite_name ) ] )
            {
               // runtime error に変更予定
               SDL_Log( "指定されたスプライトが存在していません:  %s ", name.c_str() );
               continue;
            }

            SDL_Log( "hash enemy_name : %d", u32Hash( name.c_str() ) );
            auto handle = _sprite_anim_cache.load( u32Hash( name ), anim );
         }
      }
   }

   entt::resource<SDL_Texture> ResourceManager::getSprite( std::string_view key )
   {
      if ( entt::resource<SDL_Texture> res = _sprite_cache[ u32Hash( key ) ]; res ) { return res; }
      return {};
   }
   entt::resource<SpriteAnimResource> ResourceManager::getSpriteAnim( std::string_view key )
   {
      SDL_Log( "hash enemy_name key : %d", u32Hash( key ) );
      if ( entt::resource<SpriteAnimResource> res = _sprite_anim_cache[ u32Hash( key ) ]; res ) { return res; }
      return {};
   }
}    // namespace myge