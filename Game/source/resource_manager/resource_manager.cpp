#include <SDL3/SDL.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <renderer/renderer.hpp>
#include <resource_loader/sprite_loader.hpp>
#include <resource_manager/resource_manager.hpp>

namespace
{

   using json = nlohmann::json;

   json loadAssets( std::string_view file_path_ )
   {
      // アセットファイル読み込み
      std::ifstream stream( "resource/assets/test_assets.json" );

      if ( !stream.is_open() ) { throw new std::exception( "アセットファイルを開けませんでした。" ); }

      if ( !json::accept( stream ) ) { throw new std::exception( "jsonフォーマットが不正。" ); }

      // acceptが読み進めるので頭に戻しておく
      stream.seekg( 0, std::ios::beg );

      return json::parse( stream );
   }
}    // namespace

namespace myge
{
   ResourceManager::ResourceManager() {}
   ResourceManager::~ResourceManager() {}

   void ResourceManager::loadResources( Renderer& renderer_, std::string_view assets_path_ )
   {
      // assets読み込み
      auto assets = loadAssets( assets_path_ );

      // texture読み込み
      if ( assets.contains( "Sprites" ) )
      {
         auto sprites = assets[ "Sprites" ];
         for ( const auto& sprt : sprites )
         {
            auto sprite_path = sprt[ "file" ].get<std::string>();
            if ( !std::filesystem::exists( sprite_path ) )
            {
               SDL_Log( "画像ファイルが存在しません:  %s ", sprite_path.c_str() );
               continue;
            }

            // nameからhashを生成して登録
            auto name   = sprt[ "name" ].get<std::string>();
            auto handle = _sprite_cache.load( std::hash<std::string_view>()( name ), renderer_, sprite_path );
         }
      }

      // spriteアニメーション読み込み
      if ( assets.contains( "SpriteAnims" ) )
      {
         auto sprite_anims = assets[ "SpriteAnims" ];
         for ( const auto& anim : sprite_anims )
         {
            // nameからhashを生成して登録
            auto name   = anim[ "name" ].get<std::string>();
            auto handle = _sprite_anim_cache.load( std::hash<std::string_view>()( name ), anim );
         }
      }
   }

   entt::resource<SDL_Texture> ResourceManager::getTexture( std::string_view key )
   {
      if ( entt::resource<SDL_Texture> res = _sprite_cache[ std::hash<std::string_view>()( key ) ]; res )
      {
         return res;
      }
      return entt::resource<SDL_Texture> {};
   }
}    // namespace myge