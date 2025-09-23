#pragma once
#include <core/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;

namespace myge
{
   class SpriteAnimLoader final
   {
   public:
      std::shared_ptr<SpriteAnimResource> operator()( Renderer& renderer_, json& data ) const;
   };
}    // namespace myge
