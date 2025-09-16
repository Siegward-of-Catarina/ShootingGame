#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <resource_manager.hpp>
#include <unordered_map>

using json = nlohmann::json;

namespace myge
{
   ResourceManager::ResourceManager() {}
   ResourceManager::~ResourceManager() {}

   void ResourceManager::loadResources( SDL_Renderer* const renderer_, std::string_view file_path_ )
   {
      // アセットファイル読み込み
      std::ifstream stream( "resource/assets/test_assets.json" );

      if ( !stream.is_open() ) { throw new std::exception( "アセットファイルを開けませんでした。" ); }
      if ( !json::accept( stream ) ) { throw new std::exception( "jsonフォーマットが不正。" ); }
      // acceptが読み進めるので頭に戻しておく
      stream.seekg( 0, std::ios::beg );

      json data = json::parse( stream );
      if ( data.contains( "textures" ) )
      {
         auto texs = data[ "textures" ];
         auto size { data[ "textures" ].size() };
         _textures.reserve( size );
         for ( const auto& tex : texs )
         {
            auto file_path = tex[ "file" ].get<std::string>();

            auto image = IMG_LoadTexture( renderer_, file_path.c_str() );
            if ( image == nullptr ) { SDL_Log( "画像を読み込めませんでした:  %s ", SDL_GetError() ); }
            _textures.try_emplace( tex[ "name" ], image );
         }
      }
   }

   std::shared_ptr<SDL_Texture> ResourceManager::getTexture( std::string_view key ) { return _textures[ key.data() ]; }
}    // namespace myge