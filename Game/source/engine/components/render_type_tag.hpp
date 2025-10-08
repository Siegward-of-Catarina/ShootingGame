#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine {
	struct RenderBackgroundTag {};
	struct RenderGameSpriteTag {};
	struct RenderUITag {};
	struct RenderTextTag {};
	struct RenderFadeTag {};

	void emplaceRenderTypeTag(entt::registry& registry_, entt::entity entity_, std::string_view type_);
}