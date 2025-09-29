#pragma once
namespace myge
{
   enum class OutOfScreenBeehaviorType
   {
      Destroy,
      Wrap,
      Stop,
   };
   struct OutOfScreenBehavior
   {
      OutOfScreenBeehaviorType type {};
      f32                      wrap_position_x {};
      f32                      wrap_position_y {};
   };
}    // namespace myge