#include <app/components/bounding_box.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
// event
#include <app/event/dead_event.hpp>
namespace
{
   // ソートを必要とするレイヤの判別ビットフラグ
   constexpr u8 NEED_SORT_BACKGROUND_TAG = 1;
   constexpr u8 NEED_SORT_GAMESPRITE_TAG = 2;
   constexpr u8 NEED_SORT_UI_TAG         = 4;
   constexpr u8 NEED_SORT_TEXT_TAG       = 8;
   constexpr u8 NEED_SORT_FADE_TAG       = 16;
   constexpr u8 NEED_SORT_ALL            = 31;
   u8           need_sort { 0 };

}    // namespace
namespace myge
{
   LifeCycleSystem::LifeCycleSystem( i32 priority_, entt::registry& registry_, entt::dispatcher& dispatcher_ )
     : SystemInterface { priority_, registry_ }, _dispatcher { dispatcher_ }
   {
      _dispatcher.sink<DeadEvent>().connect<&LifeCycleSystem::onEntityDead>( this );
   }
   LifeCycleSystem::~LifeCycleSystem() {}
   void LifeCycleSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> change_tag_entities {};
      for ( auto [ entity, wait ] : reg.view<WaitTag>().each() )
      {
         wait.elapsed += frame_.delta_time;
         if ( wait.elapsed > wait.wait_time ) { change_tag_entities.emplace_back( entity ); }
      }
      // 画面外で存在しているとき
      for ( auto [ entity ] : reg.view<EnteringTag>().each() )
      {
         // 登場時にソート対象にする
         need_sort = NEED_SORT_ALL;

         // バウンディングボックスを持つオブジェクトの場合
         if ( reg.all_of<BoundingBox>( entity ) )
         {
            // 一度ゲームエリア内に入ったらアクティブ状態とする
            auto& box { reg.get<BoundingBox>( entity ) };
            if ( box.state == BoundingBox::State::Inside ) { change_tag_entities.emplace_back( entity ); }
         }
         else    // その他は出現時からアクティブに
         {
            change_tag_entities.emplace_back( entity );
         }
      }
      for ( auto [ entity ] : reg.view<ActiveTag>().each() ) {}
      for ( auto [ entity ] : reg.view<DyingTag>().each() ) {}
      for ( auto [ entity ] : reg.view<DeadTag>().each() )
      {
         // 削除対象のエンティティがどのレイヤにいるか調べる
         if ( reg.all_of<sdl_engine::RenderBackgroundTag>( entity ) ) { need_sort |= NEED_SORT_BACKGROUND_TAG; }
         else if ( reg.all_of<sdl_engine::RenderGameSpriteTag>( entity ) ) { need_sort |= NEED_SORT_GAMESPRITE_TAG; }
         else if ( reg.all_of<sdl_engine::RenderUITag>( entity ) ) { need_sort |= NEED_SORT_UI_TAG; }
         else if ( reg.all_of<sdl_engine::RenderTextTag>( entity ) ) { need_sort |= NEED_SORT_TEXT_TAG; }
         else if ( reg.all_of<sdl_engine::RenderFadeTag>( entity ) ) { need_sort |= NEED_SORT_FADE_TAG; }
         reg.destroy( entity );
         // １体でも削除したら再ソート
      }

      // 降順にエンティティをソート
      if ( need_sort )
      {
         // まずはSpriteをソートこれは固定
         reg.sort<sdl_engine::Sprite>( []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
                                       { return l_sprt.render_order > r_sprt.render_order; } );
         // Spriteを基準にTransformもソートこれも固定
         reg.sort<sdl_engine::Transform, sdl_engine::Sprite>();
         // ここからは削除されたエンティティが所属していたレイヤごとにソートが必要なら
         // Spriteを基準にソートする
         if ( need_sort & NEED_SORT_BACKGROUND_TAG )
         {
            reg.sort<sdl_engine::RenderBackgroundTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_GAMESPRITE_TAG )
         {
            reg.sort<sdl_engine::RenderGameSpriteTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_UI_TAG ) { reg.sort<sdl_engine::RenderUITag, sdl_engine::Sprite>(); }
         if ( need_sort & NEED_SORT_TEXT_TAG )
         {
            reg.sort<sdl_engine::RenderTextTag, sdl_engine::Sprite>();
            reg.sort<sdl_engine::Text, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_FADE_TAG ) { reg.sort<sdl_engine::RenderFadeTag, sdl_engine::Sprite>(); }
         // ソート完了
         need_sort = 0;
      }

      for ( auto& entity : change_tag_entities )
      {
         if ( reg.all_of<WaitTag>( entity ) )
         {
            reg.remove<WaitTag>( entity );
            reg.emplace<EnteringTag>( entity );
            reg.emplace<sdl_engine::RenderableTag>( entity );
            reg.emplace<sdl_engine::UpdateableTag>( entity );
         }
         else if ( reg.all_of<EnteringTag>( entity ) )
         {

            reg.remove<EnteringTag>( entity );
            reg.emplace<ActiveTag>( entity );
            reg.emplace<sdl_engine::LogicUpdateableTag>( entity );
         }
      }
   }
   void LifeCycleSystem::onEntityDead( DeadEvent& e )
   {
      auto& reg { registry() };
      for ( auto entity : e.dead_entities )
      {
         reg.remove<ActiveTag>( entity );
         reg.emplace<DeadTag>( entity );
      }
   }
}    // namespace myge
