#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/systems/system_interface.hpp>

namespace sdl_engine
{
   struct StopSEEvent;          // SE停止要求
   struct StopBGMEvent;         // BGM停止要求（フェードアウト時間付き）
   struct StopAllSoundEvent;    // すべてのサウンド停止要求（フェードアウト時間付き）
   // サウンド再生を担うシステム
   // - SoundEffect/BackgroundMusic コンポーネントを検出して1フレームで再生要求
   // - 再生要求後は当該エンティティを破棄（使い捨てトリガー）
   class SoundSystem final : public sdl_engine::SystemInterface
   {
   public:
      SoundSystem( i32 priority_, entt::registry& registry_, EventListener& event_listener_, SoundMixer& soundmixer_ );
      virtual ~SoundSystem() override;

      // フレーム毎の更新：SE/BGMの再生キュー処理
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      // BGM停止イベントハンドラ
      void onStopBGM( const StopBGMEvent& e );
      void onStopSE( const StopSEEvent& e );
      void onStopAllSound( const StopAllSoundEvent& e );

   private:
      SoundMixer&    _soundmixer;        // 実際の再生を行うミキサ
      EventListener& _event_listener;    // イベント購読に使用
   };
}    // namespace sdl_engine
