#include <pch.hpp>
// my header
#include <app/systems/score_system.hpp>
// component
#include <app/components/entity_type_tag.hpp>
#include <app/components/score.hpp>
#include <engine/basic_component.hpp>
// event
#include <app/events/dead_event.hpp>
// session
#include <app/session/score_session.hpp>
// core
#include <engine/core.hpp>
// std
#include <format>
namespace
{
   constexpr u32 MAX_SCORE = 9999;
}
namespace myge
{
   ScoreSystem::ScoreSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
      // スコアテキストを更新
      for ( auto [ entity, text ] : registry_.view<sdl_engine::Text, ScoreTextTag>().each() )
      {
         if ( !registry_.valid( entity ) ) { continue; }
         // スコア表示を4桁にフォーマット
         text.text = std::format( "{:04}", registry_.ctx().get<ScoreSession>().value );
      }
      // deadイベントリスナー登録
      _event_listener.connect<&ScoreSystem::onScoreChanged, DeadEvent>( this );
   }
   ScoreSystem::~ScoreSystem() {}
   void ScoreSystem::update( const sdl_engine::FrameData& ) {}
   void ScoreSystem::onScoreChanged( DeadEvent& e )
   {
      auto& reg { registry() };
      auto& score_session { reg.ctx().get<ScoreSession>() };
      // 死亡エンティティにスコア加算コンポーネントがあればスコアを加算する
      for ( auto entity : e.dead_entities )
      {
         if ( !reg.valid( entity ) ) { continue; }
         // スコア加算コンポーネントを取得かつ out_of_screen_death フラグが立っていなければ加算
         if ( auto* scorebounty = reg.try_get<ScoreBounty>( entity ); scorebounty && !e.out_of_screen_death )
         {
            // セッションスコアに加算　未初期化の場合、現在更新値で初期化
            if ( reg.ctx().contains<ScoreSession>() == false )
            {
               reg.ctx().emplace<ScoreSession>( ScoreSession { scorebounty->value } );
            }
            else    // 既に初期化されている場合、加算
            {
               score_session.value += scorebounty->value;
               // スコア上限を4桁に制限
               if ( score_session.value > MAX_SCORE ) { score_session.value = MAX_SCORE; }
            }
         }
      }
      // スコアテキストを更新
      for ( auto [ entity, text ] : reg.view<sdl_engine::Text, ScoreTextTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         // スコア表示を4桁にフォーマット
         text.text = std::format( "{:04}", score_session.value );
      }
   }
}    // namespace myge