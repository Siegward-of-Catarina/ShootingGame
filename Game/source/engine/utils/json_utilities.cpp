#include <pch.hpp>
// my header
#include <engine/utils/json_utilities.hpp>
// exception
#include <engine/core/game_exception.hpp>
// stl
#include <filesystem>
#include <fstream>
namespace sdl_engine
{
   json loadJson( std::string_view assets_path_ )
   {    // アセットファイル読み込み
      const std::filesystem::path path { assets_path_ };
      std::ifstream               stream( path, std::ios::binary );

      if ( !stream.is_open() ) { throw GameException( "asset file failed opened." ); }

      if ( !json::accept( stream ) ) { throw GameException( "JSON format is invalid." ); }

      // acceptが読み進めるので頭に戻しておく
      stream.clear();
      stream.seekg( 0, std::ios::beg );

      try
      {
         return json::parse( stream );
      }
      catch ( const std::exception& e )
      {
         // 解析時の例外は詳細をそのまま渡す
         throw GameException( e.what() );
      }
   }
   std::optional<std::reference_wrapper<const json>> tryGetJson( const json& data_, std::string_view key_ )
   {
      auto d { data_.find( std::string( key_ ) ) };
      if ( d == data_.end() || d->is_null() ) { return std::nullopt; }
      return std::cref( *d );
   }
}    // namespace sdl_engine