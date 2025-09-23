#pragma once
#include <entt/entt.hpp>
#include <core/fwd.hpp>
namespace myge {
	class EntityFactory
	{
	public:
		entt::entity createEnemy(entt::registry& registry_, ResourceManager& resource_manager_, float px_, float py_);
	private:
		//ResourceManager&;
	};
}

