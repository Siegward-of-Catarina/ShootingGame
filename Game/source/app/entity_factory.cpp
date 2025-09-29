#include <app/components/bounding_box.hpp>
#include <app/components/enemy_tag.hpp>
#include <app/components/out_of_screen_behavior.hpp>
#include <app/components/player_movement.hpp>
#include <app/entity_factory.hpp>
#include <engine/components/basic_components.hpp>
#include <engine/core/game_exception.hpp>
#include <engine/entity/entity_create_helper.hpp>
#include <engine/managers/resource_manager.hpp>
#include <engine/rendering/loader/sprite_anim_resource.hpp>
#include <engine/utils/json_utilities.hpp>
namespace
{
   // Sprite�̃��\�[�X���擾���A�G���e�B�e�B��Sprite��o�^
   void getSpriteResourceAndEmplaceComponent( entt::registry&              registry_,
                                              sdl_engine::ResourceManager& resource_manager_,
                                              entt::entity                 entity_,
                                              const json&                  sprite_data_ )
   {    // SpriteResource�擾
      auto sprite_name { sdl_engine::getJsonData<std::string>( sprite_data_, "name" ) };
      auto sprite = resource_manager_.getSprite( sprite_name );

      // Sprite���G���e�B�e�B�ɓo�^
      auto sprt_comp = getSpriteComponent( sprite );
      registry_.emplace<sdl_engine::Sprite>( entity_, sprt_comp );
   }

   // SpriteAnim�̃��\�[�X���擾���A�G���e�B�e�B��SpriteAnim��o�^
   void getSpriteAnimResourceAndEmplaceComponent( entt::registry&              registry_,
                                                  sdl_engine::ResourceManager& resource_manager_,
                                                  entt::entity                 entity_,
                                                  const json&                  sprite_data_ )
   {
      // SpriterAnimResoruce�擾
      auto sprite_anim_name { sdl_engine::getJsonData<std::string>( sprite_data_, "name" ) };
      auto sprt_anim_data = resource_manager_.getSpriteAnim( sprite_anim_name );

      // SpriteAnim�����݂���Ƃ������Ƃ́ASprite�����݂���͂�
      // Sprite��dst��SpriteAnim�̃t���[���T�C�Y�ɐݒ肵�Ă���
      auto& sprt = registry_.get<sdl_engine::Sprite>( entity_ );
      sprt.dst.w = sprt_anim_data->frame_width;
      sprt.dst.h = sprt_anim_data->frame_height;

      // SpriteAnim�R���|�[�l���g���쐬
      auto sprt_anim_comp = getSpriteAnimComponent( sprt_anim_data, sprite_data_ );
      // SpriteAnim���G���e�B�e�B�ɓo�^
      registry_.emplace<sdl_engine::SpriteAnim>( entity_, sprt_anim_comp );
   }

   void emplaceBasicComponents( entt::registry&              registry_,
                                sdl_engine::ResourceManager& resource_manager_,
                                entt::entity                 entity_,
                                const json&                  data_ )
   {
      // [ Sprite�擾 ]
      if ( !data_.contains( "Sprite" ) )
      {
         throw sdl_engine::GameException( "Entity�f�[�^��sprite���܂܂�Ă��܂���" );
      }
      getSpriteResourceAndEmplaceComponent( registry_, resource_manager_, entity_, data_.at( "Sprite" ) );

      // [ Transform�擾 ]
      if ( !data_.contains( "Transform" ) )
      {
         throw sdl_engine::GameException( "Entity�f�[�^��transform���܂܂�Ă��܂���" );
      }
      auto trfm_comp = sdl_engine::getTransFormComponent( data_.at( "Transform" ) );
      registry_.emplace<sdl_engine::Transform>( entity_, trfm_comp );

      // [ Velocity�擾 ]
      if ( !data_.contains( "Velocity" ) )
      {
         throw sdl_engine::GameException( "Entity�f�[�^��velocity���܂܂�Ă��܂���" );
      }
      auto vlcy_comp = sdl_engine::getVelocityComponent( data_.at( "Velocity" ) );
      registry_.emplace<sdl_engine::Velocity>( entity_, vlcy_comp );
   }

}    // namespace
namespace myge
{
   void EntityFactory::createEntities( entt::registry&              registry_,
                                       sdl_engine::ResourceManager& resource_manager_,
                                       json&                        data_ )
   {
      std::vector<json> entities_data;
      entities_data.reserve( 20 );    // �K���Ȑ��ŗ\��
      if ( data_.contains( "Player" ) ) { entities_data.emplace_back( data_.at( "Player" ) ); }

      if ( data_.contains( "Enemy" ) )
      {
         for ( auto& enemy_data : data_.at( "Enemy" ) ) { entities_data.emplace_back( enemy_data ); }
      }

      if ( data_.contains( "BackGround" ) )
      {
         for ( auto& bg_data : data_.at( "BackGround" ) ) { entities_data.emplace_back( bg_data ); };
      }

      for ( auto& entity_data : entities_data )
      {
         auto entity = registry_.create();
         emplaceBasicComponents( registry_, resource_manager_, entity, entity_data );
         // [spriteAnim]
         if ( entity_data.contains( "SpriteAnim" ) )
         {
            getSpriteAnimResourceAndEmplaceComponent(
              registry_, resource_manager_, entity, entity_data.at( "SpriteAnim" ) );
         }
         // [BoundingBox]
         if ( entity_data.contains( "BoundingBox" ) )
         {
            auto              data = entity_data.at( "BoundingBox" );
            myge::BoundingBox box_comp { .harf_width { sdl_engine::getJsonData<u32>( data, "harf_width" ) },
                                         .harf_hegiht { sdl_engine::getJsonData<u32>( data, "harf_height" ) },
                                         .radius { sdl_engine::getJsonData<f32>( data, "radius" ) } };

            registry_.emplace<myge::BoundingBox>( entity, box_comp );
         }
         // [OutOfScreenBehavior]
         if ( entity_data.contains( "OutOfScreenBehavior" ) )
         {
            auto                      data = entity_data.at( "OutOfScreenBehavior" );
            myge::OutOfScreenBehavior behavior {};
            if ( data.at( "behavior" ) == "Destroy" ) { behavior.type = myge::OutOfScreenBeehaviorType::Destroy; }
            else if ( data.at( "behavior" ) == "Wrap" ) { behavior.type = myge::OutOfScreenBeehaviorType::Wrap; }
            else if ( data.at( "behavior" ) == "Stop" ) { behavior.type = myge::OutOfScreenBeehaviorType::Stop; }

            behavior.wrap_position_x = sdl_engine::getJsonData<f32>( data, "wrap_position_x" );
            behavior.wrap_position_y = sdl_engine::getJsonData<f32>( data, "wrap_position_y" );
            registry_.emplace<myge::OutOfScreenBehavior>( entity, behavior );
         }
      }
   }
}    // namespace myge