#include <engine/managers/resource_manager.hpp>
#include <engine/managers/scene_manager.hpp>
#include <engine/rendering/renderer.hpp>
#include <engine/scene/scene.hpp>
namespace sdl_engine
{
   Scene::Scene( GameContext& ctx_ ) : _context { ctx_ } {}

}    // namespace sdl_engine