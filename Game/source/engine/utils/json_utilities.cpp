#include <engine/core/game_exception.hpp>
#include <engine/utils/json_utilities.hpp>
#include <filesystem>
#include <fstream>
namespace sdl_engine
{
   json loadJson( std::string_view assets_path_ )
   {    // �A�Z�b�g�t�@�C���ǂݍ���
      std::ifstream stream( assets_path_.data() );

      if ( !stream.is_open() ) { throw GameException( "�A�Z�b�g�t�@�C�����J���܂���ł����B" ); }

      if ( !json::accept( stream ) ) { throw GameException( "json�t�H�[�}�b�g���s���B" ); }

      // accept���ǂݐi�߂�̂œ��ɖ߂��Ă���
      stream.seekg( 0, std::ios::beg );

      return json::parse( stream );
   }
}    // namespace sdl_engine