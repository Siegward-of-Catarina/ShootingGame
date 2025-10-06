#pragma once

#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
	class Scene
	{
	public:
		Scene(GameContext& ctx_);
		virtual ~Scene();
		virtual void initialize() = 0;
		virtual void start() = 0;
		virtual void update() = 0;

		GameContext& getGameContext() { return _context; }

	protected:
		void loadSceneData(std::string_view data_path_);
		virtual void addSystems() = 0;
		void setEntities(const std::vector<entt::entity>& entities_) { _entities = entities_; }
		std::vector<entt::entity>& getEntities() { return _entities; }
		json& getSceneData() { return _scene_data; }
	private:
		GameContext& _context;
		std::vector<entt::entity> _entities;
		json                               _scene_data;
	};
}    // namespace sdl_engine