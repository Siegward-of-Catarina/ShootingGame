#pragma once
#include <SDL3/SDL.h>
#include <engine/core/forward_declarations.hpp>
#include <engine/core/game_exception.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <source_location>
using json = nlohmann::json;

namespace sdl_engine
{
   json loadJson( std::string_view assets_path_ );
   template<typename T>
   std::optional<T> getJsonData( const json&                data_,
                                 std::string_view           key_,
                                 const std::source_location location = std::source_location::current() )
   {
      if ( !data_.contains( key_.data() ) )
      {
         std::string file_full_path { location.file_name() };
         auto        file_name { file_full_path.substr( file_full_path.rfind( '\\' ) + 1 ) };
         std::string error_msg =
           "loadJsonNotice::[" + std::string( key_ ) + "]:キーが存在しないため初期値で生成されました";
         SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, error_msg.c_str() );
         return std::nullopt;
      }
      return data_.at( key_.data() ).get<T>();
   }
}    // namespace sdl_engine