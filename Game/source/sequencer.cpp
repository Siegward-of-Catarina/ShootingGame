#include <scene.hpp>
#include <sequencer.hpp>
namespace myge
{
   Sequencer::Sequencer() : _current_scene { nullptr } {}

   Sequencer::Sequencer( Scene* current_ ) : _current_scene { current_ } {}

   Sequencer::~Sequencer() { delete _current_scene; }

   void Sequencer::deleteCurrentScene() { delete _current_scene; }

   void Sequencer::setNextScene( Scene* current_ ) { _current_scene = current_; }

   Scene* Sequencer::currentScene() { return _current_scene; }
}    // namespace myge
