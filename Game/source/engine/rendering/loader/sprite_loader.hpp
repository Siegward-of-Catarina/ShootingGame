#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   class SpriteLoader final
   {
   public:
      using result_type = std::shared_ptr<SpriteResource>;
      std::shared_ptr<SpriteResource> operator()( Renderer& renderer_, const json& data_ ) const;
   };
}    // namespace sdl_engine
