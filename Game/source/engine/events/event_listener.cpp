#include <engine/events/event_listener.hpp>

namespace sdl_engine
{
   EventListener::EventListener( entt::dispatcher& dispatcher_ ) : _dispatcher { dispatcher_ }, _connections {} {}
   EventListener::~EventListener() {}
}    // namespace sdl_engine
