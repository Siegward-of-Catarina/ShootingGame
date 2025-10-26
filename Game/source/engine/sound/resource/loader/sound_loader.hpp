#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   class SoundLoader final
   {
   public:
      using result_type = std::shared_ptr<SoundResource>;
      result_type operator()( SoundMixer& soundmixer_, const json& data_ ) const;
   };
}    // namespace sdl_engine
