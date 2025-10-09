#include <engine/Components/render_type_tag.hpp>
namespace sdl_engine {
	void emplaceRenderTypeTag(entt::registry& registry_, entt::entity entity_, std::string_view type_)
	{
		if (type_ == "Background") {
			registry_.emplace<RenderBackgroundTag>(entity_);
		}
		else if(type_ == "GameSprite") {
			registry_.emplace<RenderGameSpriteTag>(entity_);
		}
		else if (type_ == "UI") {
			registry_.emplace<RenderUITag>(entity_);
		}
		else if (type_ == "Text") {
			registry_.emplace<RenderTextTag>(entity_);
		}
		else if (type_ == "Fade") {
			registry_.emplace<RenderFadeTag>(entity_);
		}
	}
}