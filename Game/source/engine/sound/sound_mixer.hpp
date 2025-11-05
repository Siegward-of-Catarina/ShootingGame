#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   using MIX_MixerPtr = std::unique_ptr<MIX_Mixer, decltype( &MIX_DestroyMixer )>;
   using MIX_TrackPtr = std::unique_ptr<MIX_Track, decltype( &MIX_DestroyTrack )>;
   using MIX_AudioPtr = std::unique_ptr<MIX_Audio, decltype( &MIX_DestroyAudio )>;
   class SoundMixer
   {
   public:
      /**
       * @brief SE用トラック停止コールバック
       * 停止時にオーディオをクリアする
       * @param userdata_ SoundMixerのインスタンスへのポインタ
       * @param track_ 停止したトラック
       */
      static void seTrackStoppedCallback( void* userdata_, MIX_Track* track_ );
      /**
       * @brief BGM用トラック停止コールバック
       * 停止時に次のBGMがセットされていればセットして再生する
       * @param userdata_ SoundMixerのインスタンスへのポインタ
       * @param track_ 使用しない
       */
      static void bgmTrackStoppedCallback( void* userdata_, [[maybe_unused]] MIX_Track* track_ );

   private:    // inner callbacks
      void onSeTrackStopped( MIX_Track* track_ );
      void onBgmTrackStopped();

   public:
      SoundMixer( size_t se_track_size_ = 16 );
      ~SoundMixer();
      MIX_Audio* loadAudio( std::string_view path_ );
      void       setSE( MIX_Audio* audio_ );
      void       setBGM( MIX_Audio* audio_, const i64 fadeout_time_ms_ );
      /**
       * @brief setSE()されたオーディオを再生
       * @param loop_count_ ループ回数。-1で無限ループ
       * デフォルトは 0（ループなし）
       */
      void playSE( const i64 loop_count_ = 0 );
      /**
       * @brief seBGM()されたオーディオを再生
       * @param loop_count_ ループ回数。-1で無限ループ
       * デフォルトは 0（ループなし）
       */
      void playBGM( const i64 loop_count_ = 0 );
      /**
       * @brief 空きSEtrackにセットして再生
       * @param audio_ 再生するオーディオ
       * @param loop_count_ ループ回数。-1で無限ループ
       * デフォルトは 0（ループなし）
       */
      void setPlaySE( MIX_Audio* audio_, const f32 volume_, const i64 loop_count_ = 0 );
      /**
       * @brief BGMtrackにセットして再生
       * @param audio_ 再生するオーディオ
       * @param fadeout_time_ms_ フェードアウト時間（ミリ秒）
       * デフォルトは 0 (即時切替)
       * @param loop_count_ ループ回数。-1で無限ループ
       * デフォルトは 0（ループなし）
       */
      void setPlayBGM( MIX_Audio* audio_, const i64 fadeout_time_ms_ = 0, const i64 loop_count_ = 0 );
      void pauseSE();
      void pauseBGM();
      void resumeSE();
      void resumeBGM();
      void stopSE( const i64 fade_time_ms_ );
      void stopBGM( const i64 fade_time_ms_ );
      void setSEVolume( const f32 volume_ );
      void setBGMVolume( const f32 volume_ );
      void pauseAllSound();
      void resumeAllSound();
      void stopAllSound( const i64 fade_time_ms_ );

   private:
      MIX_MixerPtr              _sdl_mixer;
      MIX_TrackPtr              _bgm_track;
      MIX_Audio*                _next_bgm;
      SDL_PropertiesID          _next_bgm_properties;
      std::vector<MIX_TrackPtr> _se_tracks;
   };
}    // namespace sdl_engine