#pragma once
#include <core/fwd.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace myge
{
	class SpriteAnimLoader final
	{
	public:
		using result_type = std::shared_ptr<SpriteAnimResource>;
		std::shared_ptr<SpriteAnimResource> operator()(const json& data) const;
	};
}    // namespace myge
