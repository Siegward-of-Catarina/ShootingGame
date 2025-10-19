#pragma once
#include <engine/forward.hpp>
namespace myge
{
   struct ButtonUI
   {
      enum class Type
      {
         None,
         Start,
         Exit
      } type {};
   };
}    // namespace myge