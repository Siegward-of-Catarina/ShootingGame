#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   class EventListener
   {
   public:
      EventListener( entt::dispatcher& dispatcher_ );
      ~EventListener();
      entt::dispatcher& dispatcher() { return _dispatcher; }
      // イベント接続を安全管理
      template<auto Candidate, typename Event, typename Target>
      void connect( Target* target_ )
      {
         _connections.emplace_back( _dispatcher.sink<Event>().template connect<Candidate>( target_ ) );
      }
      // イベント即時発火
      template<typename Event>
      void trigger( Event&& event )
      {
         _dispatcher.trigger<Event>( std::forward<Event>( event ) );
      }
      // イベントキューに追加
      template<typename Event>
      void enqueue( Event&& event )
      {
         _dispatcher.enqueue<Event>( std::forward<Event>( event ) );
      }

      template<typename Event>
      bool empty()
      {
          return _dispatcher.sink<Event>().empty();
      }

      size_t size()
      {
          return _dispatcher.size();
      }

      // 接続されているイベントをすべてクリア
      void clear() { _connections.clear(); }

   public:

   private:
      entt::dispatcher&                    _dispatcher;
      std::vector<entt::scoped_connection> _connections;
   };
}    // namespace sdl_engine