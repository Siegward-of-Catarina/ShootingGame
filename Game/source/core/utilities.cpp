#include <core/utilities.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
namespace myge
{
   json loadJson( std::string_view assets_path_ )
   {
      // アセットファイル読み込み
      std::ifstream stream( assets_path_.data() );

      if ( !stream.is_open() ) { throw new std::exception( "アセットファイルを開けませんでした。" ); }

      if ( !json::accept( stream ) ) { throw new std::exception( "jsonフォーマットが不正。" ); }

      // acceptが読み進めるので頭に戻しておく
      stream.seekg( 0, std::ios::beg );

      return json::parse( stream );
   }
   void throwIfError( bool condition_, const std::string_view message_ )
   {
      throw std::runtime_error( message_.data() );
   }
}    // namespace myge
