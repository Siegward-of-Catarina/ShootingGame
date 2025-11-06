#include <pch.hpp>
// my header
#include <app/systems/enemy_ai_system.hpp>
// components
#include <app/components/enemy_boss_ai.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/components/shooter.hpp>
#include <app/components/status.hpp>
#include <engine/basic_component.hpp>
// events
#include <app/events/append_charge_effect_event.hpp>
#include <app/events/charge_end_event.hpp>
#include <app/events/dead_laser_event.hpp>
// core
#include <engine/core.hpp>
// std
#include <cmath>
namespace
{
   // 射撃プロパティ変更関数
   void changeShooterProperty( myge::MultipleShooter&  mul_shooter_,
                               myge::EnemyBossAI&      ai_,
                               i8                      num_shot_,
                               const myge::BulletType& type_ )
   {
      mul_shooter_.current_index = static_cast<u32>( type_ );
      auto& current { mul_shooter_.shooters[ mul_shooter_.current_index ] };
      current.num_shot = num_shot_;
      ai_.attack       = myge::EnemyBossAI::Attack::Shooting;
   }
}    // namespace
namespace myge
{
   EnemyBossAISystem::EnemyBossAISystem( i32                        priority_,
                                         entt::registry&            registry_,
                                         sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _random_engine {}, _event_listener { event_listener_ }
   {
      std::random_device rd;
      _random_engine.seed( rd() );

      for ( auto [ entity, ai ] : registry().view<EnemyBossAI>().each() )
      {
         // 初期ウェイト分布設定
         _distribution = std::discrete_distribution<>( ai.attack_weights.begin(), ai.attack_weights.end() );
      }

      // イベント登録
      _event_listener.connect<&EnemyBossAISystem::onChargeEnd, ChargeEndEvent>( this );
   }
   EnemyBossAISystem::~EnemyBossAISystem() {}
   void EnemyBossAISystem::update( const sdl_engine::FrameData& )
   {
      auto&                     reg { registry() };
      std::vector<entt::entity> change_tag_entity;
      for ( auto [ entity, ai, status, mul_shooter ] :
            getLogicUpdateable<EnemyBossAI, Status, MultipleShooter>( reg ).each() )
      {
         // 攻撃フェーズ管理
         switch ( ai.phase )
         {
            case EnemyBossAI::Phase::Init :
            {
               ai.phase      = EnemyBossAI::Phase::Phase1;
               _distribution = std::discrete_distribution<>( ai.attack_weights.begin(), ai.attack_weights.end() );
               break;
            }
            case EnemyBossAI::Phase::Phase1 :
            {
               if ( status.hp <= status.max_hp / 3 * 2 && ai.attack == EnemyBossAI::Attack::None )
               {
                  ai.phase          = EnemyBossAI::Phase::Phase2;
                  ai.attack_weights = { 30, 20, 30, 20, 0, 0 };
                  _distribution = std::discrete_distribution<>( ai.attack_weights.begin(), ai.attack_weights.end() );
                  ai.attack     = EnemyBossAI::Attack::LaserCharge;
               }
               break;
            }
            case EnemyBossAI::Phase::Phase2 :
            {

               if ( status.hp <= status.max_hp / 3 && ai.attack == EnemyBossAI::Attack::None )
               {
                  ai.phase          = EnemyBossAI::Phase::Phase3;
                  ai.attack_weights = { 0, 30, 10, 10, 30, 20 };
                  _distribution = std::discrete_distribution<>( ai.attack_weights.begin(), ai.attack_weights.end() );
                  ai.attack     = EnemyBossAI::Attack::LaserCharge;
               }
               break;
            }
            case EnemyBossAI::Phase::Phase3 :
            {
               // 最終フェーズなので変更なし
               break;
            }
         }

         // 攻撃選択管理
         if ( ai.attack == EnemyBossAI::Attack::None )
         {
            ai.attack = static_cast<EnemyBossAI::Attack>( _distribution( _random_engine ) );
         }

         // 攻撃内容管理
         switch ( ai.attack )
         {
            case EnemyBossAI::Attack::Normal3 :
            {
               changeShooterProperty( mul_shooter, ai, 3, BulletType::Enemy_small );
               break;
            }
            case EnemyBossAI::Attack::Normal5 :
            {
               changeShooterProperty( mul_shooter, ai, 5, BulletType::Enemy_small );
               break;
            }
            case EnemyBossAI::Attack::Big1 :
            {
               changeShooterProperty( mul_shooter, ai, 1, BulletType::Enemy_Large );
               break;
            }
            case EnemyBossAI::Attack::Big2 :
            {
               changeShooterProperty( mul_shooter, ai, 2, BulletType::Enemy_Large );
               break;
            }
            case EnemyBossAI::Attack::Big3 :
            {
               changeShooterProperty( mul_shooter, ai, 3, BulletType::Enemy_Large );
               break;
            }
            case EnemyBossAI::Attack::LaserCharge :
            {
               auto&                   trfm { reg.get<sdl_engine::Transform>( entity ) };
               sdl_engine::Vector2_f32 pos { trfm.position.x, trfm.position.y + 64.0f };
               _event_listener.trigger<AppendChargeEvent>( { pos } );
               ai.attack = EnemyBossAI::Attack::LaserCharging;
               break;
            }
            case EnemyBossAI::Attack::LaserCharging :
            {
               // 処理なし
               break;
            }
            case EnemyBossAI::Attack::LaserAttack :
            {
               if ( reg.all_of<FinishedShootTag>( entity ) ) { ai.attack = EnemyBossAI::Attack::LaserEnd; }
               if ( !reg.all_of<ShootLaserEnemyTag>( entity ) ) { change_tag_entity.emplace_back( entity ); }
               break;
            }
            case EnemyBossAI::Attack::LaserEnd :
            {
               // レーザー削除イベント
               _event_listener.trigger<DeadLaserEvent>( {} );
               ai.attack = EnemyBossAI::Attack::None;
               break;
            }
            case EnemyBossAI::Attack::None :
            {
               break;
            }
         }

         // 発射状態の確認
         if ( ai.attack == EnemyBossAI::Attack::Shooting )
         {
            if ( reg.all_of<FinishedShootTag>( entity ) ) { ai.attack = EnemyBossAI::Attack::None; }
            // ShootingDoubleBulletEnemyTagが付いていない = 射撃前 or　射撃終了後であるので付け替え対象に
            if ( !reg.all_of<ShootingDoubleBulletEnemyTag>( entity ) ) { change_tag_entity.emplace_back( entity ); }
         }
      }
      // タグの付け替え処理
      for ( auto& entity : change_tag_entity )
      {
         auto& ai = reg.get<EnemyBossAI>( entity );
         if ( reg.all_of<FinishedShootTag>( entity ) )
         {
            // 射撃終了後ならタグを外して退出状態へ
            reg.remove<FinishedShootTag>( entity );
         }
         else if ( ai.attack == EnemyBossAI::Attack::Shooting )
         {
            // 射撃前ならタグを付与して射撃開始
            reg.emplace<ShootingDoubleBulletEnemyTag>( entity );
         }
         // レーザーの場合強制的に発射する
         if ( ai.attack == EnemyBossAI::Attack::LaserAttack )
         {
            // LaserAttack状態ならタグを付与
            reg.emplace<ShootLaserEnemyTag>( entity );
         }
      }
   }
   void EnemyBossAISystem::onChargeEnd( ChargeEndEvent& )
   {
      for ( auto [ entity, ai ] : getLogicUpdateable<EnemyBossAI>( registry() ).each() )
      {
         if ( ai.attack == EnemyBossAI::Attack::LaserCharging ) { ai.attack = EnemyBossAI::Attack::LaserAttack; }
      }
   }
}    // namespace myge