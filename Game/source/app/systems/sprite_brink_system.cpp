#include <app/systems/sprite_brink_system.hpp>
#include <app/components/sprite_brink.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
#include <cmath>
namespace myge {
    SpriteBrinkSystem::SpriteBrinkSystem(i32 priority_):SystemInterface(priority_)
    {
    }
    SpriteBrinkSystem::~SpriteBrinkSystem()
    {
    }
    void SpriteBrinkSystem::update(sdl_engine::GameContext& context_)
    {
        auto delta_time{ context_.getGameTimer().getDeltaTime() };
        auto& registry{ context_.getRegistry() };

        for ( auto [entity, sprt, brink] : getLogicUpdateable<sdl_engine::Sprite, SpriteBrink>(registry).each() ) {

            switch (brink.state)
            {
            case  SpriteBrink::State::AddAlpha:
                sprt.color.a() += brink.speed * delta_time;
                if (sprt.color.a() > 1.0f )
                {
                    sprt.color.a() = 1.0f;
                    brink.state = SpriteBrink::State::SubAlpha;
                }
                break;
            case  SpriteBrink::State::SubAlpha:
                sprt.color.a() -= brink.speed * delta_time;
                if (sprt.color.a() < brink.min_alpha) 
                {
                    sprt.color.a() = brink.min_alpha;
                    brink.state = SpriteBrink::State::AddAlpha;
                }
                break;
            }
        }
    }
}
