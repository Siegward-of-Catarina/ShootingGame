#include <app/components/bounding_box.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
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
   LifeCycleSystem::LifeCycleSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   LifeCycleSystem::~LifeCycleSystem() {}
   void LifeCycleSystem::update( sdl_engine::GameContext& context_ )
   {
      //
      auto&                     registry { context_.getRegistry() };
      std::vector<entt::entity> change_tag_entities {};
      // 画面外で存在しているとき
      for ( auto [ entity ] : registry.view<EnteringTag>().each() )
      {
         // 登場時にソート対象にする
         need_sort = NEED_SORT_ALL;

         // バウンディングボックスを持つオブジェクトの場合
         if ( registry.all_of<BoundingBox>( entity ) )
         {
            // 一度ゲームエリア内に入ったらアクティブ状態とする
            auto& box { registry.get<BoundingBox>( entity ) };
            if ( box.state == BoundingBox::State::Inside ) { change_tag_entities.emplace_back( entity ); }
         }
         else    // その他は出現時からアクティブに
         {
            change_tag_entities.emplace_back( entity );
         }
      }
      for ( auto [ entity ] : registry.view<ActiveTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DyingTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DeadTag>().each() )
      {
         // 削除対象のエンティティがどのレイヤにいるか調べる
         if ( registry.all_of<sdl_engine::RenderBackgroundTag>( entity ) ) { need_sort |= NEED_SORT_BACKGROUND_TAG; }
         else if ( registry.all_of<sdl_engine::RenderGameSpriteTag>( entity ) )
         {
            need_sort |= NEED_SORT_GAMESPRITE_TAG;
         }
         else if ( registry.all_of<sdl_engine::RenderUITag>( entity ) ) { need_sort |= NEED_SORT_UI_TAG; }
         else if ( registry.all_of<sdl_engine::RenderTextTag>( entity ) ) { need_sort |= NEED_SORT_TEXT_TAG; }
         else if ( registry.all_of<sdl_engine::RenderFadeTag>( entity ) ) { need_sort |= NEED_SORT_FADE_TAG; }
         registry.destroy( entity );
         // １体でも削除したら再ソート
      }

      // 降順にエンティティをソート
      if ( need_sort )
      {
         // まずはSpriteをソートこれは固定
         registry.sort<sdl_engine::Sprite>( []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
                                            { return l_sprt.render_order > r_sprt.render_order; } );
         // Spriteを基準にTransformもソートこれも固定
         registry.sort<sdl_engine::Transform, sdl_engine::Sprite>();
         // ここからは削除されたエンティティが所属していたレイヤごとにソートが必要なら
         // Spriteを基準にソートする
         if ( need_sort & NEED_SORT_BACKGROUND_TAG )
         {
            registry.sort<sdl_engine::RenderBackgroundTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_GAMESPRITE_TAG )
         {
            registry.sort<sdl_engine::RenderGameSpriteTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_UI_TAG ) { registry.sort<sdl_engine::RenderUITag, sdl_engine::Sprite>(); }
         if ( need_sort & NEED_SORT_TEXT_TAG ) {
             registry.sort<sdl_engine::RenderTextTag, sdl_engine::Sprite>(); 
             registry.sort<sdl_engine::Text, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_FADE_TAG ) { registry.sort<sdl_engine::RenderFadeTag, sdl_engine::Sprite>(); }
         // ソート完了
         need_sort = 0;
      }

      for ( auto& entity : change_tag_entities )
      {
         if ( registry.all_of<EnteringTag>( entity ) )
         {

            registry.remove<EnteringTag>( entity );
            registry.emplace<ActiveTag>( entity );
            registry.emplace<sdl_engine::LogicUpdateableTag>( entity );
         }
      }
   }
}    // namespace myge
