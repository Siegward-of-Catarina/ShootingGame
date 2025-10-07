#include <app/components/bounding_box.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/utils.hpp>

#include <app/components/enemy_tag.hpp>
namespace
{
	// �L���ȕ����݂̂œ���������s��
	bool isInside(SDL_FRect& target_, SDL_FRect& screen_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		switch (enable_axis_)
		{
		case myge::BoundingBox::EnableAxis::Top: return target_.y >= screen_.y && target_.y <= screen_.h;
		case myge::BoundingBox::EnableAxis::Bottom: return target_.h >= screen_.y && target_.h <= screen_.h;
		case myge::BoundingBox::EnableAxis::Left: return target_.x >= screen_.x && target_.x <= screen_.w;
		case myge::BoundingBox::EnableAxis::Right: return target_.w >= screen_.x && target_.w <= screen_.w;
		case myge::BoundingBox::EnableAxis::LR: return target_.x >= screen_.x && target_.w <= screen_.w;
		case myge::BoundingBox::EnableAxis::TB: return target_.y >= screen_.y && target_.h <= screen_.h;
		case myge::BoundingBox::EnableAxis::ALL:
			return target_.y >= screen_.y && target_.h <= screen_.h && target_.x >= screen_.x && target_.w <= screen_.w;
		}
		return false;
	}
	// �L�������� Top TB ALL�̎��ɔ�����s��
	bool isPartinalTop(f32 t_top_, f32 s_top_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Top or TB or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Top))
		{
			return t_top_ < s_top_;
		}
		return false;
	}
	// �L�������� Bottom TB ALL�̎��ɔ�����s��
	bool isPartinalBottom(f32 t_bottom_, f32 s_bottom_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Bottom or TB or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Bottom))
		{
			return t_bottom_ > s_bottom_;
		}
		return false;
	}
	// �L�������� Left LR ALL�̎��ɔ�����s��
	bool isPartinalLeft(f32 t_left_, f32 s_left_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Left or LR or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Left))
		{
			return t_left_ < s_left_;
		}
		return false;
	}
	// �L�������� Right LR ALL�̎��ɔ�����s��
	bool isPartinalRight(f32 t_right_, f32 s_right_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Right or LR or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Right))
		{
			return t_right_ > s_right_;
		}
		return false;
	}
	// �L�������� Bottom TB ALL�̎��ɔ�����s��
	bool isOutTop(f32 t_bottom_, f32 s_top_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Bottom or TB or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Bottom))
		{

			return t_bottom_ < s_top_;
		}
		return false;
	}
	// �L�������� Top TB ALL�̎��ɔ�����s��
	bool isOutBottom(f32 t_top_, f32 s_bottom_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// TOP or TB or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Top))
		{
			return t_top_ > s_bottom_;
		}
		return false;
	}
	// �L�������� Right LR ALL�̎��ɔ�����s��
	bool isOutLeft(f32 t_right_, f32 s_left_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Right or LR or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Right))
		{
			return t_right_ < s_left_;
		}
		return false;
	}
	// �L�������� Left LR ALL�̎��ɔ�����s��
	bool isOutRight(f32 t_left_, f32 s_right_, myge::BoundingBox::EnableAxis& enable_axis_)
	{
		// Left or LR or ALL
		if (static_cast<u32>(enable_axis_) & static_cast<u32>(myge::BoundingBox::EnableAxis::Left))
		{
			return t_left_ > s_right_;
		}
		return false;
	}

}    // namespace
namespace myge
{
	ScreenBoundsSystem::ScreenBoundsSystem(i32 priority_) : SystemInterface(priority_) {}

	ScreenBoundsSystem::~ScreenBoundsSystem() {}

	void ScreenBoundsSystem::update(sdl_engine::GameContext& context_)
	{
		auto& registry{ context_.getRegistry() };

		for (auto [entity, box, trfm] : registry.view<BoundingBox, sdl_engine::Transform>().each())
		{

			SDL_FRect target{ .x { trfm.x - box.harf_width },
							   .y { trfm.y - box.harf_hegiht },
							   .w { trfm.x + box.harf_width },
							   .h { trfm.y + box.harf_hegiht } };

			SDL_FRect screen{ .x { 0.f },
							   .y { 0.f },
							   .w { static_cast<f32>(context_.getWindowSize().x) },
							   .h { static_cast<f32>(context_.getWindowSize().y) } };
			// ��ʊO����o�ꂷ��P�[�X�ɑΉ����邽�߁A
			// �X�e�[�g��None�̏ꍇInside������������Ȃ�
			if (box.state == BoundingBox::State::None)
			{
				// �L������[ enable_axis ]�����Ƃɔ�����s��
				if (isInside(target, screen, box.enable_axis))
				{
					box.state = BoundingBox::State::Inside;
					SDL_Log("x:%f, y:%f", trfm.x, trfm.y);
				}
			}
			else
			{
				// �L������[ enable_axis ]�����Ƃɔ�����s��
				if (isOutTop(target.h, screen.y, box.enable_axis)) { box.state = BoundingBox::State::OutTop; }
				else if (isOutBottom(target.y, screen.h, box.enable_axis))
				{

					box.state = BoundingBox::State::OutBottom;
				}
				else if (isOutLeft(target.w, screen.x, box.enable_axis)) { box.state = BoundingBox::State::OutLeft; }
				else if (isOutRight(target.x, screen.w, box.enable_axis)) { box.state = BoundingBox::State::OutRight; }
				else if (isPartinalTop(target.y, screen.y, box.enable_axis)) { box.state = BoundingBox::State::PartinalTop; }
				else if (isPartinalBottom(target.h, screen.h, box.enable_axis))
				{
					box.state = BoundingBox::State::PartinalBottom;
				}
				else if (isPartinalLeft(target.x, screen.x, box.enable_axis))
				{
					box.state = BoundingBox::State::PartinalLeft;
				}
				else if (isPartinalRight(target.w, screen.w, box.enable_axis))
				{
					box.state = BoundingBox::State::PartinalRight;
				}
				else { box.state = BoundingBox::State::Inside; }
			}
		}
	}
}    // namespace myge
