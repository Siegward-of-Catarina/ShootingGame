#pragma once
#include <core/fwd.hpp>
namespace myge
{
	class SpriteLoader final
	{
	public:
		using result_type = std::shared_ptr<SDL_Texture>;
		std::shared_ptr<SDL_Texture> operator()(Renderer& renderer_, const std::string_view path_) const;
	};
}    // namespace myge
