#include <engine/events/event_listener.hpp>

namespace sdl_engine
{
   // dispatcher 参照を保持し、接続をスコープ管理する
   EventListener::EventListener( entt::dispatcher& dispatcher_ ) : _dispatcher { dispatcher_ }, _connections {} {}
   EventListener::~EventListener() {}
}    // namespace sdl_engine