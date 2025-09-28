#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/core/game_exception.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace sdl_engine
{
   json loadJson( std::string_view assets_path_ );
   template<typename T>
   T getJsonData( const json& data_, std::string_view key_ )
   {
      if ( !data_.contains( key_.data() ) )
      {
         throw GameException( ( "éwíËÇ≥ÇÍÇΩÉLÅ[Ç™ë∂ç›ÇµÇ‹ÇπÇÒ: " + std::string( key_ ) ).c_str() );
      }
      return data_.at( key_.data() ).get<T>();
   }
}    // namespace sdl_engine