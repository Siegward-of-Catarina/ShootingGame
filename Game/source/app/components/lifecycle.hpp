#pragma once
namespace myge
{
   struct LifeCycle
   {
      enum class State
      {
         InComing,
         Active,
         Dying,
         Dead
      } state;
   };

   struct EnteringTag
   {
   };
   struct ActiveTag
   {
   };
   struct DyingTag
   {
   };
   struct DeadTag
   {
   };
}    // namespace myge