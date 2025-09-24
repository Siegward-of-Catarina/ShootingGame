#include <core/utilities.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace myge
{
   json loadJson( std::string_view assets_path_ )
   {
      // �A�Z�b�g�t�@�C���ǂݍ���
      std::ifstream stream( assets_path_.data() );

      if ( !stream.is_open() ) { throw new std::exception( "�A�Z�b�g�t�@�C�����J���܂���ł����B" ); }

      if ( !json::accept( stream ) ) { throw new std::exception( "json�t�H�[�}�b�g���s���B" ); }

      // accept���ǂݐi�߂�̂œ��ɖ߂��Ă���
      stream.seekg( 0, std::ios::beg );

      return json::parse( stream );
   }
}    // namespace myge
