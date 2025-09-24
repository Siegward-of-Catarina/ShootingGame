#include <game.hpp>
#include <renderer/renderer.hpp>
#include <resource/resource_manager.hpp>
#include <scene/scene.hpp>
#include <sequencer.hpp>
namespace myge
{
   Scene::Scene( SceneInitDesc desc_ )
     : _renderer { desc_.renderer }, _sequencer { desc_.sequencer }, _resource_manager { desc_.resource_manager }
   {
   }

   Renderer& Scene::renderer() { return _renderer; }

   ResourceManager& Scene::resourceManager() { return _resource_manager; }

}    // namespace myge