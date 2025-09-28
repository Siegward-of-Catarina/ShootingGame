#pragma once
#include <engine/core/forward_declarations.hpp>
namespace sdl_engine
{
   void throwIfError( bool condition_, const std::string_view message_ );
}    // namespace sdl_engine