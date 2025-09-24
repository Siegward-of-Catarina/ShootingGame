#pragma once
#include <core/fwd.hpp>
#include <core/core.hpp>
#include <core/json_fwd.hpp>
#include <scene/scene.hpp>
#include <entt/entt.hpp>

namespace myge
{
	class TestScene : public Scene
	{
	public:
		TestScene(SceneInitDesc desc_);
		~TestScene();
		// Scene を介して継承されました
		void proc(float _delta) override;

	private:
		entt::registry registry;
		entt::entity enemy;
		json scene_data;
	};
}    // namespace myge