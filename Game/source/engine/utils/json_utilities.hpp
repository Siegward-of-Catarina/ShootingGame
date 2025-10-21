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
   json                                              loadJson( std::string_view assets_path_ );
   std::optional<std::reference_wrapper<const json>> tryGetJson( const json& data_, std::string_view key_ );
   template<typename T>
   const T getRequireData( const json& data_, std::string_view key_ )
   {
      auto key_str = std::string( key_ );
      if ( !data_.contains( key_str ) || data_.at( key_str ).is_null() )
      {
         std::string error_msg = "loadJsonNotice::[" + key_str + "]:必須キーが存在しないため作成できません";
         throw GameException( error_msg.c_str() );
      }
      return data_.at( key_str ).get<T>();
   }
   template<typename T>
   const T
   getOptionalData( const json& data_, std::string_view key_, const T& default_value_, bool log_when_default_ = false )
   {
      auto key_str = std::string( key_ );
      if ( !data_.contains( key_str ) || data_.at( key_str ).is_null() )
      {
         if ( log_when_default_ )
         {
            std::string error_msg = "loadJsonNotice::[" + key_str + "]:キーが存在しないため初期値で生成されました";
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, error_msg.c_str() );
         }
         return default_value_;
      }
      return data_.at( key_str ).get<T>();
   }

}    // namespace sdl_engine