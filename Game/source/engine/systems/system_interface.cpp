#include <engine/systems/system_interface.hpp>
#include <engine/basic_component.hpp>
namespace sdl_engine
{
	SystemInterface::SystemInterface(i32 priority_) : _priority{ priority_ } {}
   SystemInterface::~SystemInterface() {}
   i32 SystemInterface::getPriority() const { return _priority; }
}    // namespace sdl_engine