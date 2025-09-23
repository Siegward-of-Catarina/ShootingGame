#pragma once
#include <SDL3/SDL_rect.h>
#include <core/fwd.hpp>
#include <core/core.hpp>
#include <scene.hpp>
#include <vector>
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
	};
}    // namespace myge