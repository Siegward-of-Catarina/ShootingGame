#pragma once
#include <entt/entt.hpp>
#include <core/fwd.hpp>
#include <core/json_fwd.hpp>
namespace myge {
	class EntityFactory
	{
	public:
		entt::entity createEnemy(entt::registry& registry_, ResourceManager& resource_manager_, float px_, float py_);
		entt::entity createBackGround(entt::registry& registry_, ResourceManager& resource_mamager_, const json& data_);
	private:
		//ResourceManager&;
	};
}

