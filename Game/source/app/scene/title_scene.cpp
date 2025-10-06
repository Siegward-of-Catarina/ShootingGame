#include <app/scene/title_scene.hpp>
#include <app/scene/test_scene.hpp>
#include <app/entity_factory.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>

myge::TitleScene::TitleScene(sdl_engine::GameContext& ctx_) : Scene{ ctx_ }, _scene_elapsed_time{ 0 } {}

myge::TitleScene::~TitleScene() {}

void myge::TitleScene::initialize() {
	loadSceneData("game_data/scene_data/title_scene_data.json");
	auto& scene_data{ getSceneData() };
	if (scene_data.contains("Entities")) {
		EntityFactory factory;
		setEntities(factory.createEntities(getGameContext(), scene_data.at("Entities")));
	}
	auto size{ getEntities().size() };
}

void myge::TitleScene::start() {}

void myge::TitleScene::update()
{
	f64 delta{ getGameContext().getGameTimer().getDeltaTime() };
	_scene_elapsed_time += delta;
	if (_scene_elapsed_time > 3.0f) {
		getGameContext().getSceneManager().setNextScene(std::make_unique<TestScene>(getGameContext()));
	}
}


void myge::TitleScene::addSystems()
{
}

