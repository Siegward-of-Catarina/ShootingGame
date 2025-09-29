#pragma once
#include <engine/Components/basic_components.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   SpriteAnim getSpriteAnimComponent( entt::resource<sdl_engine::SpriteAnimResource> resource_, const json& data_);
   Sprite     getSpriteComponent( entt::resource<sdl_engine::SpriteResource> resource_ );
   Transform  getTransFormComponent( const json& data_ );
   Velocity   getVelocityComponent( const json& data_ );
}    // namespace sdl_engine
