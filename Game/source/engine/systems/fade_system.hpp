#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>

namespace sdl_engine
{
   struct FadeOutStartEvent;
   struct FadeRenderLayerChangeEvent;
   struct FadeSetAlphaEvent;

   // フェード用エンティティのアルファ/描画層を制御するシステム
   class FadeSystem : public SystemInterface
   {
   public:
      FadeSystem( i32 priority_, entt::registry& registry_, EventListener& event_listener_ );
      virtual ~FadeSystem() override;

      // フェード状態に応じてアルファ/状態を更新
      virtual void update( const FrameData& frame_ ) override;

   private:
      // フェードアウト開始（各種オーバーライドを適用）
      void onFadeOutStart( FadeOutStartEvent& e );
      // フェードの描画層を切り替え（UI 下/上）
      void onFadeRenderLayerChange( FadeRenderLayerChangeEvent& e );
      // スプライトのアルファを即時設定
      void onFadeSetAlpha( FadeSetAlphaEvent& e );

   private:
      EventListener&                   _event_listener;
      std::unordered_set<entt::entity> _fades;    // 更新対象フェードの集合
   };
}    // namespace sdl_engine