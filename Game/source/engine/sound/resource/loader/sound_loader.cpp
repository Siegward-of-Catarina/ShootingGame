#include <engine/sound/resource/loader/sound_loader.hpp>
#include <engine/sound/resource/sound_resource.hpp>
#include <engine/sound/sound_mixer.hpp>
namespace sdl_engine
{
   SoundLoader::result_type SoundLoader::operator()( SoundMixer& soundmixer_, const json& data_ ) const
   {
      auto path_ = getRequireData<std::string>( data_, "file_path" );
      if ( !std::filesystem::exists( path_ ) )
      {
         std::string msg = "音声ファイルが存在しません: " + std::string( path_ );
         throw GameException( msg.c_str() );
      }

      return std::make_shared<SoundResource>( soundmixer_.loadAudio( path_ ) );
   }
}    // namespace sdl_engine