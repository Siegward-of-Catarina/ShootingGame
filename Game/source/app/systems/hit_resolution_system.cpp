// my header
#include <app/systems/hit_resolution_system.hpp>
// core
#include <engine/core.hpp>
// component
#include <app/components/damage.hpp>
#include <app/components/damage_effect_property.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/status.hpp>
// event
#include <app/event/append_dead_effect_event.hpp>
#include <app/event/hit_event.hpp>
#include <app/event/play_damage_se_event.hpp>
#include <app/event/player_life_changed_event.hpp>
namespace myge
{
   HitResolutionSystem::HitResolutionSystem( i32                        priority_,
                                             entt::registry&            registry_,
                                             sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
      _event_listener.connect<&HitResolutionSystem::onHit, HitEvent>( this );
   }
   HitResolutionSystem::~HitResolutionSystem() {}
   void HitResolutionSystem::update( const sdl_engine::FrameData& frame_ ) {}
   void HitResolutionSystem::onHit( HitEvent& e )
   {
      auto&                     reg = registry();
      std::vector<entt::entity> dead_entities;
      std::vector<entt::entity> damage_entities;
      // firstは敵側、secondはプレイヤー側と固定し扱う
      for ( auto [ enemy_side, player_side ] : e.hit_entity_pairs )
      {
         if ( reg.valid( enemy_side ) && reg.valid( player_side ) )
         {
            auto& status_a = reg.get<Status>( enemy_side );
            auto& status_b = reg.get<Status>( player_side );

            // エネミーサイドは無敵判定を取らない！！!reg.all_of<DamageEffect>( enemy_side )
            if ( status_b.atk ) { status_a.hp -= status_b.atk; }
            // 敵弾と自機の当たり判定の場合、敵弾は即死扱い
            if ( status_a.hp <= 0
                 || ( reg.all_of<EnemyBulletTag>( enemy_side ) && reg.all_of<PlayerTag>( player_side ) ) )
            {
               status_a.hp = 0;
               dead_entities.emplace_back( enemy_side );
            }
            else if ( !reg.all_of<EnemyBossLaserTag>( enemy_side ) ) { damage_entities.emplace_back( enemy_side ); }

            // プレイヤーサイドは無敵判定を取る Damage中はダメージを受けない
            if ( !reg.all_of<DamageEffect>( player_side ) && status_a.atk )
            {
               const auto before_hp = status_b.hp;
               status_b.hp -= status_a.atk;
               if ( status_b.hp <= 0 )
               {
                  status_b.hp = 0;
                  dead_entities.emplace_back( player_side );
               }
               else { damage_entities.emplace_back( player_side ); }
               // プレイヤーのライフ変化イベント発行
               if ( before_hp != status_b.hp && reg.all_of<PlayerTag>( player_side ) )
               {
                  _event_listener.trigger<PlayerLifeChangedEvent>( { status_b.hp } );
               }
            }
         }
      }
      for ( auto entity : damage_entities )
      {
         if ( !reg.valid( entity ) || reg.all_of<DamageEffect>( entity ) ) { continue; }
         // ダメージエフェクト初期値で生成
         DamageEffect damage { .red_brink_time { 0.5f }, .brink_interval { 0.5f / 7 }, .elapsed_time { 0.0f } };
         // ブリンクプロパティがあれば引き継ぐ
         if ( auto brink_prop { reg.try_get<DamageEffectProperty>( entity ) }; brink_prop )
         {
            damage.red_brink_time = brink_prop->red_brink_time;
            damage.brink_interval = brink_prop->brink_interval;
         }
         reg.emplace<DamageEffect>( entity, damage );
      }

      if ( !damage_entities.empty() )
      {
         // ダメージSE再生イベント発行
         _event_listener.trigger<PlayDamageSEEvent>( { damage_entities } );
      }
      // 死亡エフェクト生成イベント発行（空なら送らない）
      if ( !dead_entities.empty() ) { _event_listener.trigger<AppendDeadEffectEvent>( { dead_entities } ); }
   }
}    // namespace myge