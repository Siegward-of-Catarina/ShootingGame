#pragma once
#include <core/fwd.hpp>
#include <entt/entt.hpp>
#include <memory>
namespace myge
{
   class SpriteLoader final
   {
   public:
      std::shared_ptr<SDL_Texture> operator()( Renderer& renderer_, const std::string_view path_ ) const;
   };
}    // namespace myge
