// サウンド再生システム
// - SoundEffect/BackgroundMusic コンポーネントを検出してミキサへ再生依頼
// - 依頼後はエンティティを破棄（トリガーとしての一時オブジェクト）
#include <engine/systems/sound_system.hpp>
// compoenent
#include <engine/components/background_music.hpp>
#include <engine/components/soundeffect.hpp>
// event
#include <engine/events/event_listener.hpp>
#include <engine/events/sound_events.hpp>
// sound
#include <engine/sound/sound_mixer.hpp>

namespace sdl_engine
{
   SoundSystem::SoundSystem( i32             priority_,
                             entt::registry& registry_,
                             EventListener&  event_listener_,
                             SoundMixer&     soundmixer_ )
     : SystemInterface { priority_, registry_ }, _soundmixer { soundmixer_ }, _event_listener { event_listener_ }
   {
      // BGM停止イベントを購読
      _event_listener.connect<&SoundSystem::onStopBGM, StopBGMEvent>( this );
   }

   SoundSystem::~SoundSystem() {}

   void SoundSystem::update( const sdl_engine::FrameData& /*frame_*/ )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> dead_sound_entities;

      // ワンショットSEの再生要求を処理
      for ( auto [ entity, se ] : reg.view<SoundEffect>().each() )
      {
         _soundmixer.setSEVolume( se.volume );
         _soundmixer.setPlaySE( se.sound->audio, se.loop_count );
         dead_sound_entities.emplace_back( entity );    // 再生後に破棄
      }

      // BGMの再生要求を処理
      for ( auto [ entity, bgm ] : reg.view<BackgroundMusic>().each() )
      {
         _soundmixer.setBGMVolume( bgm.volume );
         _soundmixer.setPlayBGM( bgm.sound->audio, bgm.loop_count, bgm.fade_time_ms );
         dead_sound_entities.emplace_back( entity );    // 再生後に破棄
      }

      // 再生トリガーとなったエンティティをまとめて破棄
      for ( const auto& entity : dead_sound_entities ) { reg.destroy( entity ); }
   }

   void SoundSystem::onStopBGM( const StopBGMEvent& e )
   {
      // フェードアウト時間付きで停止
      _soundmixer.stopBGM( e.fadeout_time_ms );
   }
}    // namespace sdl_engine