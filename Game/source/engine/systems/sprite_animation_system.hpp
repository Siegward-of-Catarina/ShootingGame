#pragma once
#include <engine/systems/system_interface.hpp>
#include <engine/events/event_listener.hpp>
namespace sdl_engine
{
	class SpriteAnimationSystem : public SystemInterface
	{
	public:
		SpriteAnimationSystem(i32 priority_, entt::registry& registry_, EventListener& event_listener_);
		virtual ~SpriteAnimationSystem() override;
		// SystemInterface を介して継承されました
		virtual void update(const FrameData& frame_) override;

	private:
		EventListener& _event_listener;
		std::vector<entt::entity> _anim_end_entities;
	};
}    // namespace sdl_engine
