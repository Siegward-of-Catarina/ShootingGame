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
      // エネミータグ付与
      registry_.emplace<EnemyTag>( enemy );

      // ==== [ 存在するべきコンポーネントを読み込む ]============================================
      // TODO:ここ当たりをもう少しシンプルにしたい。ヘルパーのほうに少し責務を移す方向性で。
      // Sprite取得
      if ( !data_.contains( "sprite" ) ) { throw sdl_engine::GameException( "Enemyデータにspriteが含まれていません" ); }
      auto sprite_data { data_.at( "sprite" ) };

      auto sprite_name { sdl_engine::getJsonData<std::string>( sprite_data, "sprite_name" ) };
      auto sprite    = resource_manager_.getSprite( sprite_name );
      auto sprt_comp = getSpriteComponent( sprite );
      registry_.emplace<sdl_engine::Sprite>( enemy, sprt_comp );
      // SpriteAnim取得
      auto sprite_anim_name { sdl_engine::getJsonData<std::string>( sprite_data, "sprite_anim_name" ) };
      auto sprt_anim_data = resource_manager_.getSpriteAnim( sprite_anim_name );
      auto sprt_anim_comp = getSpriteAnimComponent( sprt_anim_data );
      registry_.emplace<sdl_engine::SpriteAnim>( enemy, sprt_anim_comp );

      // Transform取得
      if ( !data_.contains( "transform" ) )
      {
         throw sdl_engine::GameException( "Enemyデータにtransformが含まれていません" );
      }
      auto trfm_comp = sdl_engine::getTransFormComponent( data_ );
      registry_.emplace<sdl_engine::Transform>( enemy, trfm_comp );

      // Velocity取得
      if ( !data_.contains( "velocity" ) )
      {
         throw sdl_engine::GameException( "Enemyデータにvelocityが含まれていません" );
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

      // スプライト取得
      auto sprite_name { sdl_engine::getJsonData<std::string>( data_, "sprite_name" ) };
      auto sprite    = resource_mamager_.getSprite( sprite_name );
      auto sprt_comp = sdl_engine::getSpriteComponent( sprite );
      registry_.emplace<sdl_engine::Sprite>( bg, sprt_comp );
      // 位置情報登録
      auto trfm_comp = sdl_engine::getTransFormComponent( data_ );
      registry_.emplace<sdl_engine::Transform>( bg, trfm_comp );
      // 速度情報登録
      auto vlcy_comp = sdl_engine::getVelocityComponent( data_ );
      registry_.emplace<sdl_engine::Velocity>( bg, vlcy_comp );

      return bg;
   }
}    // namespace myge
