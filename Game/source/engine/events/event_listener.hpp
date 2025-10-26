#pragma once
#include <engine/core/forward_declarations.hpp>

namespace sdl_engine
{
   // entt::dispatcher への薄いラッパ
   // - 接続のスコープ管理（scoped_connection の保持）
   // - trigger/enqueue の簡便化
   class EventListener
   {
   public:
      EventListener( entt::dispatcher& dispatcher_ );
      ~EventListener();

      // 生の dispatcher 参照を返す
      entt::dispatcher& dispatcher() { return _dispatcher; }

      // メンバ関数のイベントハンドラを接続（安全に接続を保持）
      template<auto Candidate, typename Event, typename Target>
      void connect( Target* target_ )
      {
         _connections.emplace_back( _dispatcher.sink<Event>().template connect<Candidate>( target_ ) );
      }

      // イベントを即時発火
      template<typename Event>
      void trigger( Event&& event )
      {
         _dispatcher.trigger<Event>( std::forward<Event>( event ) );
      }

      // イベントをキューへ追加
      template<typename Event>
      void enqueue( Event&& event )
      {
         _dispatcher.enqueue<Event>( std::forward<Event>( event ) );
      }

      // シンクの接続有無
      template<typename Event>
      bool empty()
      {
         return _dispatcher.sink<Event>().empty();
      }

      // キューに積まれているイベント数
      size_t size() { return _dispatcher.size(); }

      // 接続をすべてクリア（切断）
      void clear() { _connections.clear(); }

   private:
      entt::dispatcher&                    _dispatcher;
      std::vector<entt::scoped_connection> _connections;
   };
}    // namespace sdl_engine