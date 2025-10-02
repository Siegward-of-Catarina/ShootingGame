#pragma once
namespace myge
{

   struct OutOfScreenBehavior
   {
      enum class Type
      {
         Destroy,
         Wrap,
         Stop,
         Ignore,
      } type {};
   };
}    // namespace myge