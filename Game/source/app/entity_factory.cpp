#include <app/components/enemy_tag.hpp>
#include <app/entity_factory.hpp>
#include <engine/components/basic_components.hpp>
#include <engine/core/game_exception.hpp>
#include <engine/entity/entity_create_helper.hpp>
#include <engine/managers/resource_manager.hpp>
namespace
{
}    // namespace
namespace myge
{

   entt::entity EntityFactory::createEnemy( entt::registry&              registry_,
                                            sdl_engine::ResourceManager& resource_manager_,
                                            const json&                  data_ )
   {
      auto enemy = registry_.create();
      // �G�l�~�[�^�O�t�^
      registry_.emplace<EnemyTag>( enemy );

      // ==== [ ���݂���ׂ��R���|�[�l���g��ǂݍ��� ]============================================
      // TODO:��������������������V���v���ɂ������B�w���p�[�̂ق��ɏ����Ӗ����ڂ��������ŁB
      // Sprite�擾
      if ( !data_.contains( "sprite" ) ) { throw sdl_engine::GameException( "Enemy�f�[�^��sprite���܂܂�Ă��܂���" ); }
      auto sprite_data { data_.at( "sprite" ) };

      auto sprite_name { sdl_engine::getJsonData<std::string>( sprite_data, "sprite_name" ) };
      auto sprite    = resource_manager_.getSprite( sprite_name );
      auto sprt_comp = getSpriteComponent( sprite );
      registry_.emplace<sdl_engine::Sprite>( enemy, sprt_comp );
      // SpriteAnim�擾
      auto sprite_anim_name { sdl_engine::getJsonData<std::string>( sprite_data, "sprite_anim_name" ) };
      auto sprt_anim_data = resource_manager_.getSpriteAnim( sprite_anim_name );
      auto sprt_anim_comp = getSpriteAnimComponent( sprt_anim_data );
      registry_.emplace<sdl_engine::SpriteAnim>( enemy, sprt_anim_comp );

      // Transform�擾
      if ( !data_.contains( "transform" ) )
      {
         throw sdl_engine::GameException( "Enemy�f�[�^��transform���܂܂�Ă��܂���" );
      }
      auto trfm_comp = sdl_engine::getTransFormComponent( data_ );
      registry_.emplace<sdl_engine::Transform>( enemy, trfm_comp );

      // Velocity�擾
      if ( !data_.contains( "velocity" ) )
      {
         throw sdl_engine::GameException( "Enemy�f�[�^��velocity���܂܂�Ă��܂���" );
      }
      auto vlcy_comp = sdl_engine::getVelocityComponent( data_ );
      registry_.emplace<sdl_engine::Velocity>( enemy, vlcy_comp );
      // ============================================================================

      if ( data_.contains( "leader" ) ) { auto leader_data = data_.at( "leader" ); }
      if ( data_.contains( "follower" ) ) { auto follower = data_.at( "follower" ); }

      return enemy;
   }

   entt::entity EntityFactory::createBackGround( entt::registry&              registry_,
                                                 sdl_engine::ResourceManager& resource_mamager_,
                                                 const json&                  data_ )
   {

      auto bg = registry_.create();

      // �X�v���C�g�擾
      auto sprite_name { sdl_engine::getJsonData<std::string>( data_, "sprite_name" ) };
      auto sprite    = resource_mamager_.getSprite( sprite_name );
      auto sprt_comp = sdl_engine::getSpriteComponent( sprite );
      registry_.emplace<sdl_engine::Sprite>( bg, sprt_comp );
      // �ʒu���o�^
      auto trfm_comp = sdl_engine::getTransFormComponent( data_ );
      registry_.emplace<sdl_engine::Transform>( bg, trfm_comp );
      // ���x���o�^
      auto vlcy_comp = sdl_engine::getVelocityComponent( data_ );
      registry_.emplace<sdl_engine::Velocity>( bg, vlcy_comp );

      return bg;
   }
}    // namespace myge
