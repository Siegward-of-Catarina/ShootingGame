#pragma once
#include <core/fwd.hpp>
namespace myge {
	struct SceneInitDesc {
		Renderer& renderer;
		Sequencer& sequencer;
		ResourceManager& resource_manager;
	};
}