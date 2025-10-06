#include <engine/managers/resource_manager.hpp>
#include <engine/scene/scene.hpp>
#include <engine/core.hpp>
namespace sdl_engine
{
	Scene::Scene(GameContext& ctx_) : _context{ ctx_ }
	{
	}
	Scene::~Scene() {
		auto& registry{ _context.getRegistry() };
		for (auto& entity : _entities)
		{
			if (registry.valid(entity)) { registry.destroy(entity); }
		}
	}
	void Scene::loadSceneData(std::string_view data_path_)
	{
		// jsonからシーンデータを読み込む
		_scene_data = sdl_engine::loadJson(data_path_.data());
	}

}    // namespace sdl_engine