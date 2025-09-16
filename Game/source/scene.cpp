#include <game.hpp>
#include <scene.hpp>
#include <sequencer.hpp>
namespace myge
{
   Scene::Scene( Game* game_ ) : _game { game_ } {}
   Sequencer* myge::Scene::sequencer() { return _game->sequencer(); }

   Game* Scene::game() { return _game; }

}    // namespace myge