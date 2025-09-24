#pragma once
#include <core/fwd.hpp>
namespace myge {

	class GameContext
	{
	public:
		GameContext();
		~GameContext();

	private:
		SDL_WindowPtr                    _window;
		std::unique_ptr<Renderer>        _renderer;
		std::unique_ptr<Sequencer>       _sequencer;
		std::unique_ptr<ResourceManager> _resource_manager;
	};
}

