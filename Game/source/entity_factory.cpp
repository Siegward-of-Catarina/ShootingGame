#include "entity_factory.hpp"

#include <components/sprite_anim_component.hpp>
#include <components/sprite_component.hpp>
#include <components/transform_component.hpp>
#include <components/velocity_component.hpp>

#include <resource/resource_manager.hpp>
#include <resource/sprite_anim_resource.hpp>
entt::entity myge::EntityFactory::createEnemy(entt::registry& registry_, ResourceManager& resource_manager_, float px_, float py_)
{
	auto enemy = registry_.create();

	registry_.emplace<TransFormComponent>(enemy, .0f, .0f);
	registry_.emplace<VelocityComponent>(enemy, 0.0f, 0.0f);

	//アニメーションデータ取得
	auto sprt_anim_data = resource_manager_.getSpriteAnim("enemy1_anim");
	//コンポーネント生成後エンティティに登録
	SpriteAnimComponent sprt_anim_comp
	{
		.frames{sprt_anim_data.handle()->frames},
		.frame_width{sprt_anim_data.handle()->frame_width},
		.frame_height{sprt_anim_data.handle()->frame_height},
		.frame_num{sprt_anim_data.handle()->frame_num},
		.frame_time{sprt_anim_data.handle()->frame_time},
		.is_loop{sprt_anim_data.handle()->is_loop},
		.current_frame{0},
		.elapsed_time{0.0},
		.is_playing{true}
	};

	registry_.emplace<SpriteAnimComponent>(enemy, sprt_anim_comp);

	// スプライト取得
	auto sprite = resource_manager_.getSprite(sprt_anim_data.handle()->sprite_name);//スプライト画像は存在してる

	auto sprite_w = static_cast<f32>(sprite.handle()->w);
	auto sprite_h = static_cast<f32>(sprite.handle()->h);
	SpriteComponent sprt_comp
	{
		.sprite{sprite},
		.src{0.f,0.f,sprite_w,sprite_h},
		.dst{0.f,0.f,sprite_w,sprite_h},
		.pz{1}
	};

	registry_.emplace<SpriteComponent>(enemy, sprt_comp);

	return enemy;
}

entt::entity myge::EntityFactory::createBackGround(entt::registry& registry_, ResourceManager& resource_mamager_, const json& data_)
{
	auto sprite_name{ data_.at("sprite_name").get<std::string>() };

	auto sprite = resource_mamager_.getSprite(sprite_name);

	auto sprite_w = static_cast<f32>(sprite.handle()->w);
	auto sprite_h = static_cast<f32>(sprite.handle()->h);
	SpriteComponent sprt_comp
	{
		.sprite{sprite},
		.src{0.f,0.f,sprite_w,sprite_h},
		.dst{0.f,0.f,sprite_w,sprite_h},
		.pz{ data_.at("pz").get<u16>()}
	};

	TransFormComponent trns_comp{
		.x{data_.at("px").get<f32>()},
		.y{data_.at("py").get<f32>()}
	};
	VelocityComponent vlcy_comp{
		.dx{data_.at("dx").get<f32>()},
		.dy{data_.at("dy").get<f32>()}
	};

	auto bg = registry_.create();

	registry_.emplace<SpriteComponent>(bg, sprt_comp);
	registry_.emplace<TransFormComponent>(bg, trns_comp);
	registry_.emplace<VelocityComponent>(bg, vlcy_comp);

	return bg;
}
