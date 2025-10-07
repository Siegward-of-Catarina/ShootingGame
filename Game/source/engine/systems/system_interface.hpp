#pragma once
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
	class SystemInterface
	{
	public:
		SystemInterface(i32 priority_);
		virtual ~SystemInterface();
		virtual void update(sdl_engine::GameContext& context_) = 0;
		i32          getPriority() const;
		template<typename... Components>
		auto getLogicUpdateable(entt::registry& registry_);
		template<typename... Components>
		auto getRenderable(entt::registry& registry_);

	private:
		i32 _priority;
	};

	template<typename... Components>
	inline auto SystemInterface::getLogicUpdateable(entt::registry& registry_)
	{
		return registry_.view<LogicUpdateableTag, Components...>();
	}
	template<typename... Components>
	inline auto SystemInterface::getRenderable(entt::registry& registry_)
	{
		return registry_.view<RenderableTag, Components...>();
	}
}    // namespace sdl_engine
