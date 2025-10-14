#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <engine/systems/fade_system.hpp>
#include <engine/events/fade_events.hpp>
namespace
{
	constexpr f32 MAX_ALPHA{ 1.0f };
}
namespace sdl_engine
{
	sdl_engine::FadeSystem::FadeSystem(i32 priority_, entt::registry& registry_, entt::dispatcher& dispatchar_)
		: SystemInterface{ priority_, registry_ }
		, _dispatchar{ dispatchar_ }
		, _fades{}
	{
		_dispatchar.sink<FadeOutStartEvent>().connect<&FadeSystem::onFadeOutStart>(this);
	}

	sdl_engine::FadeSystem::~FadeSystem() {}

	void sdl_engine::FadeSystem::update(const FrameData& frame_)
	{
		auto& reg{ registry() };
		for (auto& entity : _fades)
		{
			auto& fade{ reg.get<Fade>(entity) };
			auto& sprt{ reg.get<Sprite>(entity) };
			switch (fade.state)
			{
			case Fade::State::FadeIn:
				sprt.color.a() -= fade.speed * frame_.delta_time;
				if (sprt.color.a() < 0.f)
				{
					sprt.color.a() = 0.0f;
					fade.state = Fade::State::FadeInEnd;
				}
				break;
			case Fade::State::FadeOut:
				sprt.color.a() += fade.speed * frame_.delta_time;
				if (sprt.color.a() > MAX_ALPHA)
				{
					sprt.color.a() = MAX_ALPHA;
					fade.state = Fade::State::BlackOut;
				}
				break;
			case Fade::State::BlackOut:
				fade.black_out_wait += frame_.delta_time;
				if (fade.black_out_wait > fade.black_out_duration)
				{
					fade.state = Fade::State::FadeOutEnd;
					fade.black_out_wait = 0.0f;
				}
				break;
			case Fade::State::FadeInEnd: 
				fade.state = Fade::State::Idel;
				_dispatchar.trigger<FadeInEndEvent>({ entity });
				break;
			case Fade::State::FadeOutEnd:
				_dispatchar.trigger<FadeOutEndEvent>({ entity });
				// OutInの場合Inを始める
				if (fade.type == Fade::Type::OutIn) {
					fade.state = Fade::State::FadeIn;
				}
				break;
			}
		}
	}
	void FadeSystem::onFadeOutStart(FadeOutStartEvent& e)
	{
		if (registry().all_of<Fade>(e.owner)) {
			_fades.emplace_back(e.owner);
			auto& fade{ registry().get<Fade>(e.owner) };
			fade.state = Fade::State::FadeOut;
		}
	}
}    // namespace sdl_engine