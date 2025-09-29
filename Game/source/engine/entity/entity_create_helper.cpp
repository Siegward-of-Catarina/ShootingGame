#include <engine/entity/entity_create_helper.hpp>
namespace sdl_engine
{
   SpriteAnim getSpriteAnimComponent( entt::resource<sdl_engine::SpriteAnimResource> resource_, const json& data_ )
   {
      sdl_engine::SpriteAnim sprt_anim_comp { .sprite_anim { resource_ },
                                              .current_frame { data_.value( "current_frame", 0u ) },
                                              .elapsed_time { 0.0 },
                                              .is_playing { data_.value( "is_playing", true ) } };
      return sprt_anim_comp;
   }
   Sprite getSpriteComponent( entt::resource<sdl_engine::SpriteResource> resource_ )
   {
      auto               sprite = resource_;
      sdl_engine::Sprite sprt_comp {
         .texture { sprite },
         .src { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) },
         .dst { 0.f, 0.f, static_cast<f32>( sprite->texture->w ), static_cast<f32>( sprite->texture->h ) }
      };
      return sprt_comp;
   }
   Transform getTransFormComponent( const json& data_ )
   {

      sdl_engine::Transform trfm_comp { .x { sdl_engine::getJsonData<f32>( data_, "px" ) },
                                        .y { sdl_engine::getJsonData<f32>( data_, "py" ) },
                                        .angle { sdl_engine::getJsonData<f32>( data_, "angle" ) },
                                        .scale { sdl_engine::getJsonData<f32>( data_, "scale" ) } };
      return trfm_comp;
   }
   Velocity getVelocityComponent( const json& data_ )
   {
      sdl_engine::Velocity vlcy_comp { .dx { sdl_engine::getJsonData<f32>( data_, "dx" ) },
                                       .dy { sdl_engine::getJsonData<f32>( data_, "dy" ) },
                                       .anguler { sdl_engine::getJsonData<f32>( data_, "anguler" ) },
                                       .scale_rate { sdl_engine::getJsonData<f32>( data_, "scale_rate" ) } };
      return vlcy_comp;
   }
}    // namespace sdl_engine