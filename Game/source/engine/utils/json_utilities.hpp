#pragma once
#include <SDL3/SDL.h>
#include <engine/core/forward_declarations.hpp>
#include <engine/core/game_exception.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <source_location>

// nlohmann::json の別名
using json = nlohmann::json;

namespace sdl_engine
{
   // 指定パスの JSON ファイルを読み込み、json オブジェクトを返す。
   // 読み込み/解析失敗時は GameException を投げる。
   json loadJson( std::string_view assets_path_ );

   // data_ の中から key_ に対応する json 値を参照で取得する。
   // キーが存在しない、または null の場合は std::nullopt。
   std::optional<std::reference_wrapper<const json>> tryGetJson( const json& data_, std::string_view key_ );

   // 必須キーの取り出し。存在しない/値が null の場合は GameException を投げる。
   // 取得時に json::get<T>() を用いるため、T は変換可能な型である必要がある。
   template<typename T>
   const T getRequireData( const json& data_, std::string_view key_ )
   {
      auto key_str = std::string( key_ );
      if ( !data_.contains( key_str ) || data_.at( key_str ).is_null() )
      {
         std::string error_msg =
           "loadJsonNotice::[" + key_str + "] : required key does not exist and cannot be created.";
         throw GameException( error_msg.c_str() );
      }
      return data_.at( key_str ).get<T>();
   }

   // 任意キーの取り出し。存在しない/値が null の場合は default_value_ を返す。
   // log_when_default_ が true の場合、既定値使用を SDL_LogDebug に出力する。
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