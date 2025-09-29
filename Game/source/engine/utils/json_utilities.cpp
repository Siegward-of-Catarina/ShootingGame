#include <engine/core/game_exception.hpp>
#include <engine/utils/json_utilities.hpp>
#include <filesystem>
#include <fstream>
namespace sdl_engine
{
   json loadJson( std::string_view assets_path_ )
   {    // アセットファイル読み込み
      std::ifstream stream( assets_path_.data() );

      if ( !stream.is_open() ) { throw GameException( "アセットファイルを開けませんでした。" ); }

      if ( !json::accept( stream ) ) { throw GameException( "jsonフォーマットが不正。" ); }

      // acceptが読み進めるので頭に戻しておく
      stream.seekg( 0, std::ios::beg );

      return json::parse( stream );
   }
}    // namespace sdl_engine