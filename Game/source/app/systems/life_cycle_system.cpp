#include <app/components/bounding_box.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
// event
#include <app/event/dead_event.hpp>
#include <engine/events/sprite_anim_end_event.hpp>
namespace
{
   constexpr std::size_t DEFAULT_RESERVE_SIZE = 128;
}
namespace myge
{
   LifeCycleSystem::LifeCycleSystem( i32                        priority_,
                                     entt::registry&            registry_,
                                     sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }
     , _event_listener { event_listener_ }
     , _change_tag_entities {}
     , _dead_entities {}
     , _need_sort { 0 }
   {
      _change_tag_entities.reserve( DEFAULT_RESERVE_SIZE );
      _dead_entities.reserve( DEFAULT_RESERVE_SIZE );
      _event_listener.connect<&LifeCycleSystem::onEntityDead, DeadEvent>( this );
      _event_listener.connect<&LifeCycleSystem::onSpriteAnimEnd, sdl_engine::SpriteAnimEndEvent>( this );
   }
   LifeCycleSystem::~LifeCycleSystem() {}
   void LifeCycleSystem::update( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      _change_tag_entities.clear();
      _dead_entities.clear();

      if ( auto size { reg.view<DeadTag>().size() }; size > _dead_entities.capacity() )
      {
         _dead_entities.reserve( size );
      }
      if ( auto size { reg.view<WaitTag>().size() + reg.view<EnteringTag>().size() };
           size > _change_tag_entities.capacity() )
      {
         _change_tag_entities.reserve( size );
      }

      WaitEntityUpdate( frame_ );
      EnteringEntityUpdate( frame_ );
      ActiveEntityUpdate( frame_ );
      DyingEntityUpdate( frame_ );
      DeadEntityUpdate( frame_ );

      for ( auto& entity : _change_tag_entities )
      {
         if ( !reg.valid( entity ) ) { continue; }
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

      // 降順にエンティティをソート
      if ( _need_sort )
      {
         // まずはSpriteをソートこれは固定
         reg.sort<sdl_engine::Sprite>( []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
                                       { return l_sprt.render_order > r_sprt.render_order; } );
         // Spriteを基準にTransformもソートこれも固定
         reg.sort<sdl_engine::Transform, sdl_engine::Sprite>();
         // ここからは削除されたエンティティが所属していたレイヤごとにソートが必要なら
         // Spriteを基準にソートする
         if ( _need_sort & NEED_SORT_BACKGROUND_TAG )
         {
            reg.sort<sdl_engine::RenderBackgroundTag, sdl_engine::Sprite>();
         }
         if ( _need_sort & NEED_SORT_GAMESPRITE_TAG )
         {
            reg.sort<sdl_engine::RenderGameSpriteTag, sdl_engine::Sprite>();
         }
         if ( _need_sort & NEED_SORT_UI_TAG ) { reg.sort<sdl_engine::RenderUITag, sdl_engine::Sprite>(); }
         if ( _need_sort & NEED_SORT_TEXT_TAG )
         {
            reg.sort<sdl_engine::RenderTextTag, sdl_engine::Sprite>();
            reg.sort<sdl_engine::Text, sdl_engine::Sprite>();
         }
         if ( _need_sort & NEED_SORT_FADE_TAG ) { reg.sort<sdl_engine::RenderFadeTag, sdl_engine::Sprite>(); }
         // ソート完了
         _need_sort = 0;
      }
   }
   void LifeCycleSystem::WaitEntityUpdate( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity, wait ] : reg.view<WaitTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         wait.elapsed += frame_.delta_time;
         if ( wait.elapsed > wait.wait_time ) { _change_tag_entities.emplace_back( entity ); }
      }
   }
   void LifeCycleSystem::EnteringEntityUpdate( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      // 画面外で存在しているとき
      for ( auto [ entity ] : reg.view<EnteringTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         // 削除対象のエンティティがどのレイヤにいるか調べる
         if ( reg.all_of<sdl_engine::RenderBackgroundTag>( entity ) ) { _need_sort |= NEED_SORT_BACKGROUND_TAG; }
         else if ( reg.all_of<sdl_engine::RenderGameSpriteTag>( entity ) ) { _need_sort |= NEED_SORT_GAMESPRITE_TAG; }
         else if ( reg.all_of<sdl_engine::RenderUITag>( entity ) ) { _need_sort |= NEED_SORT_UI_TAG; }
         else if ( reg.all_of<sdl_engine::RenderTextTag>( entity ) ) { _need_sort |= NEED_SORT_TEXT_TAG; }

         // バウンディングボックスを持つオブジェクトの場合
         if ( reg.all_of<BoundingBox>( entity ) )
         {
            // 一度ゲームエリア内に入ったらアクティブ状態とする
            auto& box { reg.get<BoundingBox>( entity ) };
            if ( box.state == BoundingBox::State::Inside ) { _change_tag_entities.emplace_back( entity ); }
         }
         else    // その他は出現時からアクティブに
         {
            _change_tag_entities.emplace_back( entity );
         }
      }
   }
   void LifeCycleSystem::ActiveEntityUpdate( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity ] : reg.view<ActiveTag>().each() ) {}
   }
   void LifeCycleSystem::DyingEntityUpdate( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity ] : reg.view<DyingTag>().each() ) {}
   }
   void LifeCycleSystem::DeadEntityUpdate( const sdl_engine::FrameData& frame_ )
   {
      auto& reg { registry() };
      for ( auto [ entity ] : reg.view<DeadTag>().each() )
      {
         if ( !reg.valid( entity ) ) { continue; }
         _dead_entities.emplace_back( entity );
         // 削除対象のエンティティがどのレイヤにいるか調べる
         if ( reg.all_of<sdl_engine::RenderBackgroundTag>( entity ) ) { _need_sort |= NEED_SORT_BACKGROUND_TAG; }
         else if ( reg.all_of<sdl_engine::RenderGameSpriteTag>( entity ) ) { _need_sort |= NEED_SORT_GAMESPRITE_TAG; }
         else if ( reg.all_of<sdl_engine::RenderUITag>( entity ) ) { _need_sort |= NEED_SORT_UI_TAG; }
         else if ( reg.all_of<sdl_engine::RenderTextTag>( entity ) ) { _need_sort |= NEED_SORT_TEXT_TAG; }
         else if ( reg.all_of<sdl_engine::RenderFadeTag>( entity ) ) { _need_sort |= NEED_SORT_FADE_TAG; }
      }

      // まとめて削除
      for ( auto entity : _dead_entities ) { reg.destroy( entity ); }
   }
   void LifeCycleSystem::onEntityDead( DeadEvent& e )
   {
      auto& reg { registry() };
      for ( auto entity : e.dead_entities )
      {
         if ( !reg.valid( entity ) ) { continue; }
         reg.remove<ActiveTag>( entity );
         reg.emplace<DeadTag>( entity );
      }
   }
   void LifeCycleSystem::onSpriteAnimEnd( sdl_engine::SpriteAnimEndEvent& e )
   {
      auto& reg { registry() };
      for ( auto entity : e.animend_entities )
      {
         if ( !reg.valid( entity ) ) { continue; }
         reg.remove<ActiveTag>( entity );
         reg.emplace<DeadTag>( entity );
      }
   }
}    // namespace myge
