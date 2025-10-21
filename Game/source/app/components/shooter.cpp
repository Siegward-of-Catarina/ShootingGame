#include <app/components/shooter.hpp>

namespace myge
{
	Shooter createShooter(const json& data_)
	{

		f32 cooldown{ sdl_engine::getOptionalData<f32>(data_, "shoot_cooldown", 0.2f) };
		auto dir{ sdl_engine::getOptionalData<std::array<f32, 2>>(data_, "bullet_velocity",{ 0.0f, -500.0f }) };

		BulletType type{};
		auto bullet_type{ sdl_engine::getRequireData<std::string>(data_, "bullet_type") };

		if (bullet_type == "player") { type = BulletType::Player; }
		if (bullet_type == "enemy_small") { type = BulletType::Enemy_small; }
		if (bullet_type == "enemy_large") { type = BulletType::Enemy_Large; }


		Shooter shtr{ cooldown, 0.0f, dir,type };
		return shtr;
	}
	Shooter createShooter(f32 cooldown_, sdl_engine::Vector2_f32 dir, std::string_view type)
	{
		Shooter shtr{ cooldown_, 0.0f, dir };
		if (type == "player") { shtr.bullet_type = BulletType::Player; }
		if (type == "enemy_small") { shtr.bullet_type = BulletType::Enemy_small; }
		if (type == "enemy_large") { shtr.bullet_type = BulletType::Enemy_Large; }
		return shtr;
	}
}    // namespace myge