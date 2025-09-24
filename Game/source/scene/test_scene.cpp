#include <components/sprite_anim_component.hpp>
#include <components/transform_component.hpp>
#include <components/sprite_component.hpp>
#include <components/velocity_component.hpp>
#include <SDL3/SDL.h>
#include <entity_factory.hpp>
#include <game.hpp>
#include <renderer/renderer.hpp>
#include <resource/resource_manager.hpp>
#include <scene/test_scene.hpp>
#include <scene/data/background_data.hpp>
#include <core/utilities.hpp>
#include <nlohmann/json.hpp>

namespace
{
	float    frame{ 0 };
	uint32_t max_frame{ 8 };
	float    x{ 200 };
	float    y{ 800 };

}    // namespace

namespace myge
{
	TestScene::TestScene(SceneInitDesc desc_) : Scene{ desc_ }, enemy{ 0 }
	{
		resourceManager().loadResources(renderer(), "assets/test_assets.json");
		EntityFactory factory;
		enemy = factory.createEnemy(registry, resourceManager(), 0, 0);

		scene_data = loadJson("scene_data/game_scene_data.json");

		if (scene_data.contains("BackGrounds")) {
			for (const auto bd_data : scene_data["BackGrounds"])
			{
				auto back = factory.createBackGround(registry, resourceManager(), bd_data);
			}
		}

		registry.sort<SpriteComponent>([](const SpriteComponent& l_sprt, const SpriteComponent& r_sprt){return l_sprt.pz > r_sprt.pz; });
		for (auto [entity, sprt] : registry.view<SpriteComponent>().each())
		{
			SDL_Log("%d", sprt.pz);
		}

	}
	TestScene::~TestScene() {}
	void myge::TestScene::proc(float _delta)
	{
		renderer().setRenderClearColor(.3f, .3f, .3f, 1.f);

		renderer().RenderClear();

		for (auto [entity, trfm, velo] : registry.view<TransFormComponent, VelocityComponent>().each())
		{
			trfm.x += velo.dx * _delta;
			trfm.y += velo.dy * _delta;
			if (trfm.y > 800.0f) {
				trfm.y = -799.0f;
			}
		}

		for (auto [entity, trfm, sprt] : registry.view<TransFormComponent, SpriteComponent>().each())
		{
			sprt.dst.x = trfm.x;
			sprt.dst.y = trfm.y;
		}

		for (auto [entity, sprt, anim] : registry.view<SpriteComponent, SpriteAnimComponent>().each())
		{
			frame += _delta * 30.0f;
			auto idx = static_cast<u32>(frame) % max_frame;

			sprt.src = anim.frames[idx];
			sprt.dst.w = anim.frame_width;
			sprt.dst.h = anim.frame_height;
		}

		for (auto [entity, sprt] : registry.view<SpriteComponent>().each())
		{
			renderer().RenderTexture(sprt.sprite.handle().get(), &sprt.src, &sprt.dst);
		}

		renderer().RenderPresent();
	}
}    // namespace myge
