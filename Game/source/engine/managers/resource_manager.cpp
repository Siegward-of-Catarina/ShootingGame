#include <SDL3/SDL.h>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/loader/sprite_anim_resource.hpp>
#include <engine/rendering/loader/sprite_resource.hpp>
#include <engine/rendering/renderer.hpp>
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
         auto sprites = assets[ "Sprites" ];

         // ルートパスを取得。末尾が「 / 」じゃない場合追加する
         auto sprite_root_path = sprites[ "SpriteRootPath" ].get<std::string>();
         if ( !sprite_root_path.ends_with( '/' ) ) { sprite_root_path += '/'; }

         auto spritedatas = sprites[ "SpriteDatas" ];
         for ( auto& sprt : spritedatas )
         {
            // pathを連結させる
            sprt[ "file_path" ] = sprite_root_path + sprt.at( "file_path" ).get<std::string>();

            // nameからhashを生成して登録
            auto name   = sprt.at( "name" ).get<std::string>();
            auto handle = _sprite_cache.load( u32Hash( name ), renderer_, sprt );
         }
      }

      // spriteアニメーション読み込み
      if ( assets.contains( "SpriteAnims" ) )
      {
         auto sprite_anims = assets[ "SpriteAnims" ];
         for ( const auto& anim : sprite_anims )
         {

            // nameからhashを生成して登録
            auto name = anim.at( "name" ).get<std::string>();

            SDL_Log( "hash enemy_name : %d", u32Hash( name.c_str() ) );
            auto handle = _sprite_anim_cache.load( u32Hash( name ), anim );
         }
      }
   }

   entt::resource<SpriteResource> ResourceManager::getSprite( std::string_view key )
   {
      if ( entt::resource<SpriteResource> res = _sprite_cache[ u32Hash( key ) ]; res ) { return res; }
      return {};
   }
   entt::resource<SpriteAnimResource> ResourceManager::getSpriteAnim( std::string_view key )
   {
      SDL_Log( "hash enemy_name key : %d", u32Hash( key ) );
      if ( entt::resource<SpriteAnimResource> res = _sprite_anim_cache[ u32Hash( key ) ]; res ) { return res; }
      return {};
   }
}    // namespace sdl_engine