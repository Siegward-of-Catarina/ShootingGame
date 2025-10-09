#pragma once
#include <engine/core/forward_declarations.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace sdl_engine
{
   class SpriteAnimLoader final
   {
   public:
      using result_type = std::shared_ptr<SpriteAnimResource>;
      result_type operator()( const json& data ) const;
   };
}    // namespace sdl_engine
