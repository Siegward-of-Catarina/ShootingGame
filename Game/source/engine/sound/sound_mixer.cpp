// my header
#include <engine/sound/sound_mixer.hpp>
// core
#include <engine/core.hpp>
namespace
{
   constexpr const char* SE_TAG  = "se";
   constexpr const char* BGM_TAG = "bgm";
}    // namespace
namespace sdl_engine
{
   void SoundMixer::seTrackStoppedCallback( void* userdata_, MIX_Track* track_ )
   {
      auto mixer = static_cast<SoundMixer*>( userdata_ );
      mixer->onSeTrackStopped( track_ );
   }
   void SoundMixer::bgmTrackStoppedCallback( void* userdata_, MIX_Track* track_ )
   {
      auto mixer = static_cast<SoundMixer*>( userdata_ );
      mixer->onBgmTrackStopped();
   }
   void SoundMixer::onSeTrackStopped( MIX_Track* track_ ) { MIX_SetTrackAudio( track_, nullptr ); }
   void SoundMixer::onBgmTrackStopped()
   {
      // 次のBGMがセットされていればセットして再生
      if ( _next_bgm )
      {
         MIX_SetTrackAudio( _bgm_track.get(), _next_bgm );
         MIX_PlayTrack( _bgm_track.get(), _next_bgm_properties );
         _next_bgm = nullptr;
      }
   }

   SoundMixer::SoundMixer( size_t se_track_size_ )
     : _sdl_mixer { nullptr, nullptr }
     , _bgm_track { nullptr, nullptr }
     , _se_tracks {}
     , _next_bgm { nullptr }
     , _next_bgm_properties {}
   {
      // ミキサーの初期化
      if ( !MIX_Init() ) { throw GameException( "SDL_Mixerの初期化に失敗しました" ); }

      // ミキサー生成
      auto mixer { MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr ) };
      assert( mixer != nullptr );
      _sdl_mixer = { mixer, &MIX_DestroyMixer };

      // SEトラック生成
      _se_tracks.reserve( se_track_size_ );
      for ( size_t i { 0 }; i < se_track_size_; i++ )
      {
         auto track { MIX_CreateTrack( _sdl_mixer.get() ) };
         MIX_TagTrack( track, SE_TAG );    // SEタグ付与
         MIX_SetTrackStoppedCallback( track, SoundMixer::seTrackStoppedCallback, this );
         _se_tracks.emplace_back( track, &MIX_DestroyTrack );
      }
      // BGMトラック生成
      {
         auto bgm_track { MIX_CreateTrack( _sdl_mixer.get() ) };
         MIX_TagTrack( bgm_track, BGM_TAG );    // BGMタグ付与
         MIX_SetTrackStoppedCallback( bgm_track, SoundMixer::bgmTrackStoppedCallback, this );
         _bgm_track = { bgm_track, &MIX_DestroyTrack };
      }
   }
   SoundMixer::~SoundMixer()
   {
      _next_bgm = nullptr;
      _se_tracks.clear();
      _bgm_track.reset();
      _sdl_mixer.reset();
      MIX_Quit();
   }
   MIX_Audio* SoundMixer::loadAudio( std::string_view path_ )
   {
      auto audio { MIX_LoadAudio( _sdl_mixer.get(), path_.data(), true ) };
      if ( !audio )
      {
         std::string msg = "音声ファイルのロードに失敗しました: " + std::string( SDL_GetError() );
         throw GameException( msg.c_str() );
      }
      return audio;
   }
   void SoundMixer::setSE( MIX_Audio* audio_ )
   {
      // 空いているトラックにセット
      for ( auto& track : _se_tracks )
      {
         if ( !MIX_TrackPlaying( track.get() ) )
         {
            MIX_SetTrackAudio( track.get(), audio_ );
            break;
         }
      }
   }
   void SoundMixer::setBGM( MIX_Audio* audio_, const i64 fadeout_time_ms_ )
   {
      // fadeout_time_ms_が0以下なら即座に切り替え
      if ( fadeout_time_ms_ <= 0 ) { MIX_SetTrackAudio( _bgm_track.get(), audio_ ); }
      else
      {
         _next_bgm = audio_;
         // msをフレームに変換してフェードアウト開始
         MIX_StopTrack( _bgm_track.get(), MIX_TrackMSToFrames( _bgm_track.get(), fadeout_time_ms_ ) );
      }
   }
   void SoundMixer::playSE( const i64 loop_count_ )
   {
      for ( auto& track : _se_tracks )
      {
         // オーディオが有効かつ再生中でないトラックを再生
         if ( MIX_GetTrackAudio( track.get() ) && !MIX_TrackPlaying( track.get() ) )
         {
            // ループ回数セット
            auto properties { MIX_GetTrackProperties( track.get() ) };
            SDL_SetNumberProperty( properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop_count_ );
            // 　再生
            MIX_PlayTrack( track.get(), properties );
            break;
         }
      }
   }
   void SoundMixer::playBGM( const i64 loop_count_ )
   {
      // ループ回数セット
      auto properties { MIX_GetTrackProperties( _bgm_track.get() ) };
      SDL_SetNumberProperty( properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop_count_ );
      MIX_PlayTrack( _bgm_track.get(), properties );
   }
   void SoundMixer::setPlaySE( MIX_Audio* audio_, const i64 loop_count_ )
   {
      // 空いているトラックにセット
      for ( auto& track : _se_tracks )
      {
         if ( !MIX_TrackPlaying( track.get() ) )
         {
            MIX_SetTrackAudio( track.get(), audio_ );
            // ループ回数セット
            auto properties { MIX_GetTrackProperties( track.get() ) };
            SDL_SetNumberProperty( properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop_count_ );
            // 　再生
            MIX_PlayTrack( track.get(), properties );
            break;
         }
      }
   }
   void SoundMixer::setPlayBGM( MIX_Audio* audio_, const i64 fadeout_time_ms_, const i64 loop_count_ )
   {
      // fadeout_time_ms_が0以下なら即座に切り替え
      if ( fadeout_time_ms_ <= 0 || !MIX_TrackPlaying( _bgm_track.get() ) )
      {
         MIX_SetTrackAudio( _bgm_track.get(), audio_ );
         // ループ回数セット
         auto properties { MIX_GetTrackProperties( _bgm_track.get() ) };
         SDL_SetNumberProperty( properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop_count_ );
         MIX_PlayTrack( _bgm_track.get(), properties );
      }
      else
      {
         _next_bgm = audio_;
         // 次BGMのループ回数セット
         _next_bgm_properties = MIX_GetTrackProperties( _bgm_track.get() );
         SDL_SetNumberProperty( _next_bgm_properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop_count_ );
         // msをフレームに変換してフェードアウト開始
         MIX_StopTrack( _bgm_track.get(), MIX_TrackMSToFrames( _bgm_track.get(), fadeout_time_ms_ ) );
      }
   }
   // SE/BGMの一時停止、再開、停止 BGM用trackが増える場合を加味してタグでの制御にしておく　増えるとは言ってない
   void SoundMixer::pauseSE() { MIX_PauseTag( _sdl_mixer.get(), SE_TAG ); }
   void SoundMixer::pauseBGM() { MIX_PauseTag( _sdl_mixer.get(), BGM_TAG ); }

   void SoundMixer::resumeSE() { MIX_ResumeTag( _sdl_mixer.get(), SE_TAG ); }
   void SoundMixer::resumeBGM() { MIX_ResumeTag( _sdl_mixer.get(), BGM_TAG ); }

   void SoundMixer::stopSE( const i64 fadeout_time_ms_ ) { MIX_StopTag( _sdl_mixer.get(), SE_TAG, fadeout_time_ms_ ); }
   void SoundMixer::stopBGM( const i64 fadeout_time_ms_ )
   {
      MIX_StopTag( _sdl_mixer.get(), BGM_TAG, fadeout_time_ms_ );
   }

   void SoundMixer::pauseAllSound() { MIX_PauseAllTracks( _sdl_mixer.get() ); }
   void SoundMixer::resumeAllSound() { MIX_ResumeAllTracks( _sdl_mixer.get() ); }

   void SoundMixer::stopAllSound( const i64 fadeout_time_ms_ )
   {
      MIX_StopAllTracks( _sdl_mixer.get(), fadeout_time_ms_ );
   }

   void SoundMixer::setSEVolume( const f32 volume_ ) { MIX_SetTagGain( _sdl_mixer.get(), SE_TAG, volume_ ); }
   void SoundMixer::setBGMVolume( const f32 volume_ ) { MIX_SetTagGain( _sdl_mixer.get(), BGM_TAG, volume_ ); }

}    // namespace sdl_engine
