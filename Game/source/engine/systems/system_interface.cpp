#include <engine/basic_component.hpp>
#include <engine/systems/system_interface.hpp>

namespace sdl_engine
{
   SystemInterface::SystemInterface( i32 priority_, entt::registry& registry_ )
     : _priority { priority_ }, registry_ { registry_ }
   {
   }
   SystemInterface::~SystemInterface() {}
}    // namespace sdl_engine