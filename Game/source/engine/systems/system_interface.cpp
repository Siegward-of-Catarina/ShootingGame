#include <pch.hpp>
// my header
#include <engine/systems/system_interface.hpp>
// component
#include <engine/basic_component.hpp>

namespace sdl_engine
{
   // priority と registry を受け取り、共通機能を提供
   SystemInterface::SystemInterface( i32 priority_, entt::registry& registry_ )
     : _priority { priority_ }, registry_ { registry_ }
   {
   }
   SystemInterface::~SystemInterface() {}
}    // namespace sdl_engine