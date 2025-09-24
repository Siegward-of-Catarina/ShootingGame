#pragma once
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;
namespace myge
{
   json loadJson( std::string_view assets_path_ );
   void throwIfError( bool condition_, const std::string_view message_ );
}    // namespace myge