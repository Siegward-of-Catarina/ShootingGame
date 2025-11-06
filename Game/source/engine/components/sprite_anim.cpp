#include <pch.hpp>
// my header
#include <engine/components/sprite_anim.hpp>
// manager
#include <engine/managers/resource_manager.hpp>
namespace sdl_engine
{
   SpriteAnim createSpriteAnim( ResourceManager& resource_manager_, const json& data_ )
   {
      // SpriterAnimResoruce取得
      auto sprite_anim_name { sdl_engine::getRequireData<std::string>( data_, "name" ) };
      auto sprt_anim_data { resource_manager_.getSpriteAnim( sprite_anim_name ) };
      auto current_frame { sdl_engine::getOptionalData<u32>( data_, "current_frame", 0u ) };
      auto is_playing { sdl_engine::getOptionalData<bool>( data_, "is_playing", true ) };

      sdl_engine::SpriteAnim sprt_anim_comp { .sprite_anim { sprt_anim_data },
                                              .current_frame { current_frame },
                                              .elapsed_time { 0.0 },
                                              .is_playing { is_playing } };
      return sprt_anim_comp;
   }
   SpriteAnim createSpriteAnim( entt::resource<SpriteAnimResource> resource_, u32 current_frame_, bool is_playing_ )
   {
      return SpriteAnim { resource_, current_frame_, 0.0, is_playing_ };
   }
}    // namespace sdl_engine